#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <QtCore/QObject>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

class TcpServer : public QObject {
    Q_OBJECT

public:
    explicit TcpServer(int tcp_port);
    ~TcpServer() {};

private slots:
    void onDisconnected();
    void onNewConnection();
    void onReadyRead();

private:
    QTcpServer* m_server;
    QTcpSocket* m_socket;
};

#endif