//#include <opencv2/opencv.hpp>
#include "QtCore/QSaveFile"
#include "QtCore/QDir"
#include <ctime>

/*
qint64 getTimeStamp(){
    struct timespec time_now;
    (void) clock_gettime(CLOCK_REALTIME, &time_now);
    qint64 qUnix = time_now.tv_sec * 1000000000 + time_now.tv_nsec;
    return qUnix;
}
*/
 

std::string timestr() {
       //std::stringstream strm; strm << t; return strm.str();
       QString qstrTime = QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz"); 
       return qstrTime.toStdString();
    }
    
std::string daystr() {
    QString qstrTime = QDateTime::currentDateTime().toString("yyyyMMdd"); 
    return qstrTime.toStdString();
}

std::string hourstr() {
    QString qstrHour = QDateTime::currentDateTime().toString("hh"); 
    return qstrHour.toStdString();
}

int saveImage(QString filename, QByteArray image){
          std::cout << "Save defect image: " << filename.toStdString() << "............." << std::endl;
          QString sPath = filename.left(filename.lastIndexOf('/'));
          if(!QFile::exists(filename)){
                  QDir dir(sPath);
                  if(!dir.exists(sPath))
                  {   
                  //路径不存在，则创建路径
                  if(!dir.mkpath(sPath))
                          SPDLOG_ERROR("Create path {} failed!!!", sPath.toStdString());//路径创建失败就没办法了，通常不会失败
                  }   
          }   
          QSaveFile file(filename);
          file.open(QIODevice::WriteOnly);
          file.write(image);
          // Calling commit() is mandatory, otherwise nothing will be written.
          file.commit();
          return 0;
  }

