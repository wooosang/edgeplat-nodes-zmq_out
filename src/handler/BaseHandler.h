//
// Created by HP on 2022/5/11.
//

#ifndef NODETEMPLATE_BASEHANDLER_H
#define NODETEMPLATE_BASEHANDLER_H
#include <vector>
#include "DataFrame.h"
#include "zmq.h"

class BaseHandler {
private:
    std::vector<void *> *subscribers_sockets;

public:
    BaseHandler(){}
    virtual void parseConfig(QVariantMap configMap){}
    void setSubscriberSocks(std::vector<void *> &sub_socks);
    virtual void handle(data_frame *df);
    virtual QByteArray doHandle(data_frame *df);
    virtual void reload(){}
    virtual void close(){}
};


#endif //NODETEMPLATE_BASEHANDLER_H
