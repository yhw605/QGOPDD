#ifndef BATCHDOWNLOAD_H
#define BATCHDOWNLOAD_H

#include <QObject>
#include <QThread>
#include "ftpdownloader.h"
#include "downloadfinished.h"
// #include "qgopdd.h"

class BatchDownload : public QThread
{
  Q_OBJECT
public:
  friend class QGOPDD;  
  BatchDownload();
  void run() override {
    StartDownloading(curr_date_, end_date_, daydiff_);
  }
  QStringList waiting_list;
  void StartDownloading(QDate curr_date, QDate end_date,
                        int daydiff);
signals:
  // void DownloadHasStarted();
  void DownloadHasFinished();
private:
  QString working_directory_ = "";
  FtpDownloader* ftp_downloader = new FtpDownloader();
  QString GetWd() {return working_directory_;}
  void SetWd(QString wd) {working_directory_ = wd;}
  QMap<QString, bool> download_type_{
      {"BRDM", false},
      {"BRD4", false},
      {"CLK", false},
      {"EPH", false},
      {"SP3", false},
      {"ION", false},
      {"DCB", false}
  };
  QDate curr_date_, end_date_;
  int  daydiff_;
  DownloadFinished* finish_widget_ = new DownloadFinished();
};

#endif // BATCHDOWNLOAD_H
