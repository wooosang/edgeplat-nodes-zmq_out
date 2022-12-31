//
// Created by HP on 2022/5/25.
//

#ifndef VARTRANSFORMER_DATAFRAME_H
#define VARTRANSFORMER_DATAFRAME_H
#include <QtCore/QObject>
#include <QtNetwork/QTcpServer>

struct data_frame{
    QString context;
    QByteArray image;

    //友元函数 实现QDataStream operation<<
    friend QDataStream &operator<<(QDataStream &output , const data_frame &dataFrame)
    {
        output<<dataFrame.context<<dataFrame.image;
        return output;
    }
    //友元函数 实现QDataStream operation>>
    friend QDataStream &operator>>(QDataStream &input , data_frame &dataFrame)
    {
        input>>dataFrame.context>>dataFrame.image;
        return input;
    }

    //友元函数 QDebug operation<<
    friend QDebug operator<<(QDebug debug, const data_frame &dataFrame)
    {
        QDebugStateSaver saver(debug);
        debug.nospace() << dataFrame.context << Qt::endl;
        return debug;
    }
};

#endif //VARTRANSFORMER_DATAFRAME_H
