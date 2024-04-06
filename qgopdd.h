#ifndef QGOPDD_H
#define QGOPDD_H

#include <QMainWindow>
#include <QtWebEngineWidgets>
#include <QtCore>
#include <QStringConverter>
#include <QVector>
#include <QMap>
#include <QNetworkAccessManager>
#include <QWebChannel>
#include <tuple>
#include <QDate>

#include "calendarwidget.h"
#include "progresscaller.h"
#include "ftpdownloader.h"
#include "batchdownload.h"
#include "textviewer.h"
#include "downloadfinished.h"
// #include "fineftp/server.h"

class Stations {
public:
  Stations(QString name, qreal lat, qreal lon, qreal height)
      : name_(name), lat_(lat), lon_(lon), height_(height) { }
  Stations() {}
  auto GetInfo() -> std::tuple<QString, qreal, qreal, qreal> {return {name_, lat_, lon_, height_};}
  void SetInfo(QString name, qreal lat, qreal lon)  {
    name_ = name;
    lat_ = lat;
    lon_ = lon;
  }
private:
  QString name_ = "";
  qreal lat_ = 0.0, lon_ = 0.0, height_ = -1e5;
};

class DataDate { // Storing the begin and end of data waiting for downloading
public:
  DataDate(QDate date) : date_(date) {this->doy_ = date.dayOfYear(), this->year_ = date.year();}
  DataDate(int year, int doy) : doy_(doy), year_(year){}
  DataDate() {}
  int GetYear() {return year_;}
  int GetDayOfYear() {return doy_;}
  void SetYear(int yy) {this->year_ = yy;}
  void SetDayOfYear(int doy) {this->doy_ = doy;}
  QDate GetDate() {return date_;}
  void SetDate(QDate d) {this->date_ = d;}
private:
  int doy_ = 0, year_ = 0;
  QDate date_;
  int gps_week = 0;
};

QT_BEGIN_NAMESPACE
namespace Ui {
class QGOPDD;
class IgsNetwork;
}
QT_END_NAMESPACE

class QGOPDD : public QMainWindow {
  Q_OBJECT

public:
  friend class BatchDownload;
  QGOPDD(QWidget *parent = nullptr);
  ~QGOPDD();
  // std::tuple<QString, qreal, qreal>

  BatchDownload* ToBatchDownload();

  void AddStationsComboBox(QJsonDocument igs_file);

  QMap<QString, Stations> igs_stations;

  DataDate begin, end;

  QWebEnginePage* igs_page;

  QStringList waiting_list;

  QStringList GetWaitingDownloadList();

  void SetDownloadType(QString type) {
    this->download_type_[type] = true;
  }

  QVector<bool> CheckDownloadType(QStringList products) {
    QVector<bool> ret;
    for (auto product_type : products) {
      if (this->download_type_[product_type]) {
        ret.append(this->download_type_[product_type]);
      }
    }
    return ret;
  }

  void StartDownloading(QDate &curr_date, QDate &end_date,
                        int &daydiff);

public slots:
  void GetEndDateSelected(QDate date);

  void GetBeginDateSelected(QDate date);

private slots:
  void on_BtnAddSta_clicked();

  void on_BtnEndCalender_clicked(bool checked);

  void on_BtnBeginCalendar_clicked(bool checked);

  void on_BtnGotoStation_clicked(bool check);
  

  void on_BtnStartDownload_clicked();

  void on_BtnDownloadDir_clicked();

  void on_CheckBoxBrdm_stateChanged(int arg1);

  void on_CheckBoxBrd4_stateChanged(int arg1);

  void on_CheckBoxSp3_stateChanged(int arg1);

  void on_BtnOpenDownloadDir_clicked();

private:
  Ui::QGOPDD *ui;
  QString working_directory_ = "";
  CalendarWidget* begincalender = new CalendarWidget(),* endcalendar = new CalendarWidget();
  ProgressCaller* progress_caller = new ProgressCaller();
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
  TextViewer* text_viewer_ = new TextViewer();
  DownloadFinished* finished_widget_ = new DownloadFinished();
};

class IgsNetwork : public QWidget {

};

QJsonDocument LoadIgsStationsFromJson(QString path);

#endif  // QGOPDD_H
