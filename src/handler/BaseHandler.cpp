//
// Created by HP on 2022/5/11.
//

#include "BaseHandler.h"
#include <iostream>

//void BaseHandler::parseConfig(QVariantMap configMap){
//	std::cout << "BaseHandler parse config!!!" << std::endl;
//}

void BaseHandler::setSubscriberSocks(std::vector<void *> &sub_socks){
	std::cout << "Base begin set subscribers" << std::endl;
    subscribers_sockets = &sub_socks;
}

QByteArray BaseHandler::doHandle(data_frame *df){
    std::cout << "Received some data....." << std::endl;
    if(!(df->image.isNull())){
    	std::cout << "Received bytes length:" <<  df->image.length() << std::endl;
    }
    return NULL;
}

void BaseHandler::handle(data_frame *df){
    std::cout << "BaseHandler do handle.................." << std::endl;
    QByteArray data = doHandle(df);
    if(subscribers_sockets != NULL){
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
