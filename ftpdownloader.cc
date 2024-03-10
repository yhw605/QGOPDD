#include "ftpdownloader.h"
#include <QEventLoop>

FtpDownloader::FtpDownloader(QObject *parent)
    : QObject{parent}
{}

void FtpDownloader::DownloadFtp(QString path) {
  QNetworkAccessManager manager;
  QUrl url{"ftp://igs.gnsswhu.cn/"};
  // url.setScheme("ftp");
  url.setUserName("anonymous");
  url.setPassword("");
  // url.setHost("igs.gnsswhu.cn");
  url.setPort(21211);
  url.setPath("/pub/gps/data/daily/2023/198/23d/ABMF00GLP_R_20230100000_01D_30S_MO.crx.gz");
  QNetworkRequest request(url);
  // /* 执行Get请求 */
  QNetworkReply* reply = manager.get(request);
  QEventLoop eventLoop;
  QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
  /* 进入等待，但事件循环依然进行 */
  eventLoop.exec();

  if (reply->error() == QNetworkReply::NoError) {
    /* 创建本地文件 */
    QFile file("ABMF00GLP_R_20230100000_01D_30S_MO.crx.gz");
    file.open(QIODevice::WriteOnly);
    /* 写入文件 */
    file.write(reply->readAll());
    file.close();
  }
  else {
    qDebug() << "Error: " << reply->errorString();
  }
}
