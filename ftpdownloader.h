#ifndef FTPDOWNLOADER_H
#define FTPDOWNLOADER_H
#include <QUrl>
#include <QNetworkAccessManager>
#include <QFile>
#include <QNetworkReply>
#include <QVector>
// #include "curl/curl.h"


class FtpDownloader : public QObject
{
  Q_OBJECT
public:
  friend class QGOPDD;
  explicit FtpDownloader(QObject *parent = nullptr);
  void DownloadGnssObs(QString path, QString url, QString sta_name,
                   QString username = "anonymous", QString password = "");
  QStringList GetFileList() {
    return file_list_;
  }

signals:

private:
  QStringList file_list_;
  void FetchFileList(QString ftp_addr, QString username = "", QString password = "");
};

#endif // FTPDOWNLOADER_H
