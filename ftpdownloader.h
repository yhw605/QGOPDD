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

  // void DownloadGnssNav()

  QStringList GetFileList() {
    return file_list_;
  }
  static int ProgressCallback(const char* flag, double dltotal, double dlnow,
                              double ultotal, double ulnow);

  static int GetDownloadFilelength(const char* curl);

  void SetWd(QString wd) {download_dir_ = wd;}

  QString GetWd() {return this->download_dir_;}

  void ClearFilelist() {this->file_list_.clear();}

signals:

private:
  QStringList file_list_;
  QString download_dir_;
  double download_file_len_ = .0, local_file_len = .0;
  void FetchFileList(QString ftp_addr, QString filename = "", QString username = "", QString password = "");
};

#endif // FTPDOWNLOADER_H
