#include "calendarwidget.h"
#include "ui_calendarwidget.h"

CalendarWidget::CalendarWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CalendarWidget)
{
  ui->setupUi(this);
  ui->CalendarWidgetSele->setLocale(QLocale::English);
  // connect(ui, &QCalendarWidget::clicked, signal_mapper,
          // static_cast<void(QSignalMapper::*)(const QString &)>(&QSignalMapper::map));
}

CalendarWidget::~CalendarWidget()
{
  delete ui;
}

void CalendarWidget::on_buttonBox_accepted()
{
  year = this->GetYear();
  doy = this->GetDayOfYear();
  QDate date = ui->CalendarWidgetSele->selectedDate();
  emit this->DateSelected(date);
  this->close();
  // delete this;
}

int CalendarWidget::GetYear() {
  QDate date = ui->CalendarWidgetSele->selectedDate();
  return date.year();
}

int CalendarWidget::GetDayOfYear() {
  QDate date = ui->CalendarWidgetSele->selectedDate();
  return date.dayOfYear();
}

void CalendarWidget::on_buttonBox_rejected()
{
  delete this;
}

