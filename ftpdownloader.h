#ifndef FTPDOWNLOADER_H
#define FTPDOWNLOADER_H
#include <QUrl>
#include <QNetworkAccessManager>
#include <QFile>
#include <QNetworkReply>


class FtpDownloader : public QObject
{
  Q_OBJECT
public:
  explicit FtpDownloader(QObject *parent = nullptr);
  void DownloadFtp(QString path);

signals:
};

#endif // FTPDOWNLOADER_H
