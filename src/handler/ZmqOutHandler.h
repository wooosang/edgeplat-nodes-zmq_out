//
// Created by HP on 2022/5/11.
//

#include "BaseHandler.h"

class ZmqOutHandler : public BaseHandler {
private:
    std::vector<void *> *subscribers_sockets = {};
    QString m_face = "0";
    int m_width;
    int m_height;
    int m_mode;
    bool m_nodrop;
    bool m_nosave;
    bool m_review = false;
    bool m_unionall = false;
    QString m_brandno = "10000";
    QString m_model_prefix;

public:
    ZmqOutHandler();
    void setSubscriberSocks(std::vector<void *> &sub_socks);
    void handle(data_frame *df);
    void parseConfig(QVariantMap configMap);
    void close();
    QByteArray doHandle(data_frame *df, bool& isOk);
    QVariantMap parseJson(QByteArray resultBytes);
};


