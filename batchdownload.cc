#include "batchdownload.h"
#include <QTimer>

void Crx2Rnx(QString);

BatchDownload::BatchDownload() {}

void BatchDownload::StartDownloading(QDate curr_date, QDate end_date, int daydiff) {
  // QThread download_thread;
  // FtpDownloader* ftp = (this->ftp_downloader);
  // ftp->moveToThread(&download_thread);
  // emit DownloadHasStarted();
  while (curr_date <= end_date) {
    int doy = curr_date.dayOfYear();
    int year = curr_date.year();
    QString syear = QString::number(year).mid(2, 2);

    QString url =
        "ftp://igs.gnsswhu.cn/pub/gps/data/daily/" + QString::number(year) +
        "/" + QString::number(doy).rightJustified(3, '0') + "/" + syear + "d/";
    this->ftp_downloader->FetchFileList(url);
    QString path =
        this->GetWd() + "/" + QString::number(doy).rightJustified(3, '0');
    int wait_size = waiting_list.size();
    int i = 0;
    // progress_bar->setRange(0, wait_size);
    // progress_bar->show();
    // progress_bar->update();
    // progress_bar.te

    for (auto sta : waiting_list) {
      this->ftp_downloader->DownloadGnssObs(path, url, sta);
      i += 1;
      double progress = static_cast<double>(i) / (daydiff * wait_size);
      // progress_bar->setFormat("Date: " + curr_date.toString(Qt::ISODate) +
      //                         ",Downloading station: " + sta + " %v/%m");
      // progress_bar->setValue(progress * 100);
      // progress_bar->update();
    }

    this->ftp_downloader->ClearFilelist();
    url[url.size() - 2] = 'p';
    this->ftp_downloader->FetchFileList(url, "SHA512SUMS-NAV");
    if (download_type_["BRDM"]) {
      this->ftp_downloader->DownloadGnssObs(path, url, "BRDM");
    }
    if (download_type_["BRD4"]) {
      this->ftp_downloader->DownloadGnssObs(path, url, "BRD4");
    }
    curr_date = curr_date.addDays(1);
    this->ftp_downloader->ClearFilelist();
    this->ftp_downloader->ExtractGzFile(path);
    Crx2Rnx(path);
    // QProcess::start("./thirdparty/hatanaka/rnxcmp/source/CRX2RNX", {}) ;
  }
  // QTimer* timer = new QTimer();
  // auto w = new DownloadFinished();
  // connect(timer, &QTimer::timeout, w, [=](){
  //   w->close();
  //   timer->stop();
  // });
  // timer->start(5000);
  emit DownloadHasFinished();
}
