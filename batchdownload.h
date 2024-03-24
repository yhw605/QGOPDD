#ifndef BATCHDOWNLOAD_H
#define BATCHDOWNLOAD_H

#include <QObject>
#include "ftpdownloader.h"
// #include "qgopdd.h"

class BatchDownload
{
public:
  friend class QGOPDD;
  BatchDownload();
  QStringList waiting_list;
  void StartDownloading(QDate &curr_date, QDate &end_date,
                        int &daydiff);
signals:
  void DownloadHasStarted();
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
};

#endif // BATCHDOWNLOAD_H
