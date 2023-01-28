#include <string>
#include "ZmqOutHandler.h"
#include <iostream>
#include <QtCore/QBuffer>
#include <QtGui/QImageReader>
#include <QtGui/QPixmap>
#include <QtCore/QJsonDocument>
#include "spdlog/spdlog.h"
#include <QtConcurrent/QtConcurrent>
//#include <sys/time.h>
#include <QtCore/QDateTime>
#include "tools.hpp"
#include <sys/stat.h>
#include <chrono>

using namespace std;

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
    auto start = std::chrono::high_resolution_clock::now();
	
    if(!(df->image.isNull())){
        std::cout << std::endl << "Received image bytes length:" <<  df->image.length() << std::endl;
    }
    
    string check_result = "";

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = end - start;
    printf("Duration : %f", duration.count());
	dataMap["node_check_end"] = duration.count();

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
    if(!data.isNull() && subscribers_sockets != NULL && !subscribers_sockets->empty()){
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

