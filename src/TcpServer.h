#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <QtCore/QObject>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include "zmq.h"
#include "handler/BaseHandler.h"

using namespace std;

class TcpServer : public QObject {
    Q_OBJECT

public:
    explicit TcpServer(int tcp_port);
    ~TcpServer() {};
    BaseHandler * initHandler();

signals:
    void close();

public slots:
    void closeServer();
private slots:
    void onDisconnected();
    void onNewConnection();
    void onReadyRead();

private:
    QTcpServer* m_server;
//    QTcpSocket* m_socket;
    QMap<QString,int> command_map;
    vector<string> subscribers;
    vector<void *> subscribers_sockets;
    void *pull_context;
    void *push_context;
    BaseHandler *m_handler;
    QThread *m_thread = nullptr;

    void sendMsg();
    void init_command_map();
    void subscribe(QVariantMap commandMap);
    void config(QVariantMap commandMap);
    void start(QVariantMap commandMap);
//    void do_start(void * context, string endpoint,BaseHandler *handler);
    void stop(QVariantMap commandMap);
};


#endif

//  Convert C string to 0MQ string and send to socket
static int
s_send (void *socket, char *string) {
    int size = zmq_send (socket, string, strlen (string), 0);
    return size;
}

//  Receive 0MQ string from socket and convert into C string
//  Caller must free returned string. Returns NULL if the context
//  is being terminated.
static char *
s_recv (void *socket) {
    enum { cap = 256 };
    char buffer [cap];
    int size = zmq_recv (socket, buffer, cap - 1, 0);
    if (size == -1)
        return NULL;
    buffer[size < cap ? size : cap - 1] = '\0';


    // remember that the strdup family of functions use malloc/alloc for space for the new string.  It must be manually
    // freed when you are done with it.  Failure to do so will allow a heap attack.
}
