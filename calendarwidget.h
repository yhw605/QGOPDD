#ifndef CALENDARWIDGET_H
#define CALENDARWIDGET_H

#include <QWidget>
#include <QSignalMapper>
#include <QDate>

namespace Ui {
class CalendarWidget;
}

class CalendarWidget : public QWidget
{
  Q_OBJECT

public:
  explicit CalendarWidget(QWidget *parent = nullptr);
  ~CalendarWidget();
  int GetYear();
  int GetDayOfYear();
  int year, doy;

signals:
  void DateSelected(QDate date);

private slots:
  // void on_dateSelected(const QDate &date);

  void on_buttonBox_accepted();

  void on_buttonBox_rejected();

private:
  Ui::CalendarWidget *ui;
  QSignalMapper *signal_mapper;
};

#endif // CALENDARWIDGET_H
