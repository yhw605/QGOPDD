#ifndef FTPDOWNLOADER_H
#define FTPDOWNLOADER_H
#include <QUrl>
#include <QNetworkAccessManager>
#include <QFile>
#include <QNetworkReply>
#include <QVector>
#include <mutex>
#include <QThread>
const QString MINIGZIP_PATH = "./thirdparty/zlib/minigzip ";
const QString CRX2RNX_PATH = "./thirdparty/hatanaka/rnxcmp/source/CRX2RNX ";
// #include "curl/curl.h"


class FtpDownloader : public /*QObject,*/ QThread
{
  Q_OBJECT
public:
  friend class QGOPDD;
  friend class BatchDownload;
  explicit FtpDownloader(QThread *parent = nullptr);
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

  void ExtractGzFile(QString path, QString sta_name = "");

  bool download_complete = false;
  std::mutex ftp_mtx;

signals:
  void ProgressChanged(int progress);
  void Finished();

public slots:
  void DownloadProgress() {
    progress_ += 1;
    emit(ProgressChanged(progress_));
  }

private:
  QStringList file_list_;
  QString download_dir_;
  double download_file_len_ = .0, local_file_len = .0;
  void FetchFileList(QString ftp_addr, QString filename = "",
                     QString username = "", QString password = "");
  int progress_ = 0;
};

#endif // FTPDOWNLOADER_H
