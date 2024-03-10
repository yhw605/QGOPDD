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
  DataDate(int year, int doy) : doy_(doy), year_(year){}
  DataDate() {}
  int GetYear() {return year_;}
  int GetDayOfYear() {return doy_;}
  void SetYear(int yy) {this->year_ = yy;}
  void SetDayOfYear(int doy) {this->doy_ = doy;}
private:
  int doy_ = 0, year_ = 0;
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
  QGOPDD(QWidget *parent = nullptr);
  ~QGOPDD();
  // std::tuple<QString, qreal, qreal>

  void AddStationsComboBox(QJsonDocument igs_file);

  QMap<QString, Stations> igs_stations;

  DataDate begin, end;

  QWebEnginePage* igs_page;

public slots:
  void GetEndDateSelected(QDate date);

  void GetBeginDateSelected(QDate date);

private slots:
  void on_BtnAddSta_clicked();

  void on_BtnEndCalender_clicked(bool checked);

  void on_BtnBeginCalendar_clicked(bool checked);



  void on_BtnGotoStation_clicked(bool check);

private:
  Ui::QGOPDD *ui;
  CalendarWidget* begincalender = new CalendarWidget(),* endcalendar = new CalendarWidget();
};

class IgsNetwork : public QWidget {

};

QJsonDocument LoadIgsStationsFromJson(QString path);

#endif  // QGOPDD_H
