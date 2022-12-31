#include <string>
#include "ZmqOutHandler.h"
#include <iostream>
#include <QtCore/QBuffer>
#include <QtGui/QImageReader>
#include <QtGui/QPixmap>
#include <QtCore/QJsonDocument>
#include "spdlog/spdlog.h"
#include <QtConcurrent/QtConcurrent>
#include <sys/time.h>
#include <QtCore/QDateTime>
#include "tools.hpp"
#include <sys/stat.h>

using namespace std;
using namespace cv;
int nreleaseEngines();
int initEnv(string base_dir, char* prd_id, int logOut, int computeMode);
string nievaluate(const string md_prd_id, cv::Mat &img);


ZmqOutHandler::ZmqOutHandler(){
	printf("ZmqOutHandler init.\n");
 }

void ZmqOutHandler::parseConfig(QVariantMap configMap){
    std::cout << "Begin parse config." << std::endl;
    bool unionall = configMap["unionall"].toBool();
    QString model_prefix = configMap["model_prefix"].toString();

    printf("Config parse succeed. Begin reload.\n");
 }


void ZmqOutHandler::setSubscriberSocks(std::vector<void *> &sub_socks){
	subscribers_sockets = &sub_socks; 
}


QByteArray ZmqOutHandler::doHandle(data_frame *df, bool & isOk){
	QVariantMap dataMap;
	qint64 qGetDataUnix = getTimeStamp();
	
    if(!(df->image.isNull())){
        std::cout << std::endl << "Received image bytes length:" <<  df->image.length() << std::endl;
    }
    
    string check_result = "";
	struct timespec time_begin_check;
	(void) clock_gettime(CLOCK_REALTIME, &time_begin_check);
	qint64 qBeginCheckUnix = time_begin_check.tv_sec * 1000000000 + time_begin_check.tv_nsec;
	qint64 qPrepareEvlCost = qBeginCheckUnix - qGetDataUnix;
	dataMap["prepare_evaluate_cost"] = qPrepareEvlCost / 1000000;
	long long st = getCurrentTimeMsec();
	long long ed = getCurrentTimeMsec();
	printf(" Evaluate total time=%llu ms \n", ed - st);
	
	struct timespec time_node_end;
	(void) clock_gettime(CLOCK_REALTIME, &time_node_end);
	qint64 qNodeEndUnix = time_node_end.tv_sec * 1000000000 + time_node_end.tv_nsec;
	dataMap["node_check_end"] = qNodeEndUnix;

	QJsonDocument jdoc = QJsonDocument::fromVariant(dataMap);
	QByteArray data;
    QDataStream outd(&data, QIODevice::ReadWrite);
    outd << QString(jdoc.toJson());
	qDebug() << jdoc.toJson();
	return data;
}

void ZmqOutHandler::handle(data_frame *df){
    bool isOk = true;
    QByteArray data = doHandle(df, isOk);
    std::cout << "Subscribers: " << subscribers_sockets->size() << std::endl;
    if(data != NULL && subscribers_sockets != NULL && !subscribers_sockets->empty()){
        std::cout << "Ready to push to subscribers: " << subscribers_sockets->size() << std::endl;
        for (int i = 0; i < subscribers_sockets->size(); ++i) {
                void *sub_sock = subscribers_sockets->at(i);
                zmq_msg_t msg;
                zmq_msg_init_size(&msg, data.size());
                memcpy(zmq_msg_data(&msg), data, data.size());
                int rc = zmq_msg_send(&msg, sub_sock, ZMQ_DONTWAIT);
                zmq_msg_close(&msg);
        }
    }
}

void ZmqOutHandler::close(){
    spdlog::debug("ZmqOutHandler closing.......\n");
}

