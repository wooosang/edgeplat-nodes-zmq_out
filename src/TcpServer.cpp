#include "TcpServer.h"
#include <iostream>
#include <QtCore/QByteArray>
#include "spdlog/spdlog.h"
#include <QtCore/QJsonDocument>
#include "QtCore/QJsonObject"
#include <QtCore/QThread>
#include "zmq.h"
#include <thread>
#include <QtCore/QDataStream>
#include <QtCore/QFile>
#include <algorithm>
#include "handler/ZmqOutHandler.h"

using namespace std;
extern bool _stop=false;


void TcpServer::init_command_map(){
    command_map.insert("config", 1);
    command_map.insert("subscribe", 2);
    command_map.insert("consume",3);
    command_map.insert("start",4);
    command_map.insert("stop",5);
}



TcpServer::TcpServer(int tcp_port)
{
    QObject::connect(this, SIGNAL(close()), this, SLOT(closeServer()));
    m_server = new QTcpServer;
    bool b_listen = m_server->listen(QHostAddress::Any, tcp_port);
    if(b_listen){
        std::cout << "Start listen on port "<<tcp_port<<" succeed!"<<std::endl;
    }else{
        qDebug() << m_server->errorString();
        std::cout << "Start listen on port "<<tcp_port<<"failed!"<<m_server->errorString().toStdString()<<std::endl;
    }
    connect(m_server, SIGNAL(newConnection()), this,SLOT(onNewConnection()));
    push_context = zmq_init(1);
    pull_context = zmq_init(1);

//    init_command_map();
 }

 BaseHandler* TcpServer::initHandler(){
    printf("Being init handler.\n");
    ZmqOutHandler *handler = new ZmqOutHandler();
    handler->setSubscriberSocks(subscribers_sockets);
    printf("Handler init succeed!\n");
    return handler;
}

void TcpServer::onNewConnection()
{
//    m_socket = m_server->nextPendingConnection();
    QTcpSocket* v_socket = m_server->nextPendingConnection();

    connect(v_socket, SIGNAL(disconnected()), SLOT(onDisconnected()));
    connect(v_socket, SIGNAL(readyRead()), SLOT(onReadyRead()));
    std::cout<<"New connection accepted!"<<std::endl;
}

QVariantMap parseMessage(QByteArray messageBytes)
{
    QJsonDocument doc;
    doc = QJsonDocument::fromJson(messageBytes);
    QJsonObject jObject = doc.object();
    QVariantMap mainMap = jObject.toVariantMap();
    return mainMap;
}

void TcpServer::sendMsg()
{
    void *context = zmq_ctx_new();
    void *client = zmq_socket(context, ZMQ_REQ);
    assert (client);
    zmq_connect (client, "tcp://localhost:12345");
    zmq_send (client, "helloworld", strlen("helloworld"), 0);
}

int parseCommand(std::string command_string){
    if(command_string == "config"){
        return 0;
    }
    if(command_string == "subscribe"){
        return 1;
    }
    if(command_string == "start"){
        return 2;
    }
    if(command_string == "send"){
        return 3;
    }
    if(command_string == "stop"){
        return 5;
    }
    return -1;
}

void TcpServer::config(QVariantMap configMap)
{
    printf("Ready to config.\n");
	if(m_handler == nullptr){
            printf("Begin init handler.\n");
		m_handler = initHandler();
            printf("Init handler succeed.\n");
	}

	m_handler->parseConfig(configMap);
//  	m_handler->reload();
}



void TcpServer::subscribe(QVariantMap commandMap)
{
    std::string endpoint = commandMap["endpoint"].toString().toStdString();

    if(subscribers.size()>0 && *find(subscribers.begin(),subscribers.end(),endpoint)==endpoint){
        cout<<"Endpoint already exists!"<<endl;
    }else{
        subscribers.push_back(endpoint);
        void *push = zmq_socket(push_context, ZMQ_PUSH);
        subscribers_sockets.push_back(push);
        zmq_bind(push, endpoint.c_str());
//        cout<<"Subscribe ["<<endpoint<<"] succeed!"<<endl;
//        printf("Subscribe %s succeed!\n", endpoint);
    }
}

void do_start(void * context, string endpoint,BaseHandler *handler){
    void *pull = zmq_socket(context,ZMQ_PULL);
    int rc = zmq_connect(pull, endpoint.c_str());
    int queue_length = 20;
    //设置队列长度，用于缓冲
    zmq_setsockopt(pull,ZMQ_RCVHWM, &queue_length,sizeof(queue_length));
    int iTimeOut = 300; //ms
    zmq_setsockopt(pull, ZMQ_RCVTIMEO, &iTimeOut, sizeof(iTimeOut));

    cout<<"Listen on "<<endpoint<<endl;
    while(!_stop){
         zmq_msg_t msg;
         zmq_msg_init(&msg);
         int rc = zmq_msg_recv(&msg,pull,0);
         if(-1 !=rc)
         {
             char *ptr= (char*)(zmq_msg_data(&msg));
             QByteArray send_frame = QByteArray::fromRawData(ptr,rc);
             QDataStream intd(&send_frame,QIODevice::ReadWrite);
             data_frame df;
             intd>>df;
             qDebug() << "Received: " << df.context << endl;

             handler->handle(&df);
             zmq_msg_close(&msg);
         }

    }
    zmq_close(pull);
    handler->close();
//    delete handler;
//    handler = nullptr;
    std::cout << "Worker pull zmq message exit!......" << std::endl;
}

void TcpServer::start(QVariantMap commandMap){
    std::string endpoint = commandMap["endpoint"].toString().toStdString();
    if(m_handler == nullptr){
        m_handler = initHandler();
    }
    _stop=false;
    m_thread = QThread::create(do_start, pull_context, endpoint, m_handler);
    m_thread->start();
}

void TcpServer::stop(QVariantMap commandMap){
    std::cout << "Stopping.................." << std::endl;
    _stop = true;
   // m_handler->close();
    for(int i=0; i < subscribers_sockets.size(); i++){
      void* push_sock = subscribers_sockets[i];
      zmq_close(push_sock);
    }
    subscribers_sockets.clear();
    subscribers.clear();
    
    /*
    try{
    if(m_thread->isRunning()){
	    m_thread->quit();
    }
    }catch(...){
    }*/
    //delete m_handler;
    //m_handler = nullptr;
    std::cout<<"Stop succeed!" << std::endl;
}
void TcpServer::onReadyRead()
{
    QTcpSocket *v_socket = qobject_cast<QTcpSocket*>(sender());
//    QTcpSocket v_socket = QObject::sender;
    QByteArray readedBytes = v_socket->readAll();
    std::string msg = readedBytes.toStdString();
    SPDLOG_INFO("Received {}", msg);
    cout<<"Received: "<<msg<<endl;
    QVariantMap msgMap = parseMessage(readedBytes);
    QString command =  msgMap["command"].toString();
    printf("Execute %s command\n", command.toStdString().c_str());
//    int command_value = command_map[command.toStdString()];
    int result = 0;
    try{
    switch(parseCommand(command.toStdString())){
        case 0:
            config(msgMap);
            break;
        case 1:
            subscribe(msgMap);
            cout << "Subscribe succeed!" <<endl;
            printf("Subscribe succeed!\n");
            break;
        case 2:
            start(msgMap);
            break;
	case 5:
	    stop(msgMap);
	    break;
    }
    }catch(int e){
      result = 1;
      printf("Something wrong!!!\n");
      cerr << "Something wrong happend!!!"<<endl;
    }catch(string e){
      printf("Ready to send exception to manager. \n");
      QByteArray output_result;
      output_result.resize(1);
//      string msg = "异常了，模型目录不对!";
      string msg(e);
//      output_result[0] = sizeof(msg);
      output_result[0] = msg.length();
      v_socket->write(output_result);
      v_socket->write(QByteArray::fromStdString(msg));
      return;
    }
    
    printf("Ready to send %d to manager.\n", result);
    QByteArray output_result;
    output_result.resize(1);
    output_result[0] = result;
    v_socket->write(output_result);

}

void TcpServer::onDisconnected()
{
    std::cout<<"Client disconnected!";
}

void TcpServer::closeServer()
{
    m_server->close();    
    zmq_close(pull_context);
    zmq_close(push_context);
}
