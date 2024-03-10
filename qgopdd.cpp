#include "qgopdd.h"
#include "./ui_qgopdd.h"
#include "calendarwidget.h"
#include "ftpdownloader.h"

#include <QWebChannel>
#include <QWebEnginePage>
#include <QTimer>
// #include "fineftp/server.h"

QGOPDD::QGOPDD(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::QGOPDD)
{
  ui->setupUi(this);
  // ui->TableWidgetAddedStas->horizontalHeader()->sectionResizeMode(QHeaderView::Stretch);
  ui->ComboSeleSta->setEditable(true);
  auto igs_json_file = LoadIgsStationsFromJson("igs_stations.json");
  this->AddStationsComboBox(igs_json_file);
  ui->TableWidgetAddedStas->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  QUrl igs_network{"https://network.igs.org"};
  ui->IgsNetworkWebWidget->setUrl(igs_network);
  ui->IgsNetworkWebWidget->load(igs_network);
  igs_page = new QWebEnginePage(ui->IgsNetworkWebWidget);
  igs_page->setUrl(igs_network);
  qDebug() << ui->IgsNetworkWebWidget->url();
  ui->IgsNetworkWebWidget->setPage(igs_page);
  FtpDownloader d;
  d.DownloadFtp("");
  // qDebug() << igs_page->url();

  // Connect the widget and the igs network
  // QWebEnginePage* igs_page = new QWebEnginePage(ui->IgsNetworkWebWidget);
  // ui->IgsNetworkWebWidget->setPage(igs_page);

  qDebug() << connect(endcalendar, SIGNAL(DateSelected(QDate)),
          this, SLOT(GetEndDateSelected(QDate)));
  qDebug() << connect(begincalender, SIGNAL(DateSelected(QDate)),
                      this, SLOT(GetBeginDateSelected(QDate)));
  // qDebug() << connect()
  // ui->CalendarWidgetSelectDate->setLocale(QLocale::English);
  // connect(maincalender, SIGNAL(DateSelected(QDate)), this, SLOT(GetBeginDateSelected(QDate)));
}

QGOPDD::~QGOPDD()
{
  delete ui;
}

void QGOPDD::AddStationsComboBox(QJsonDocument igs_file)
{
  auto igs_sta_obj = igs_file.array();
  // QJsonValue igs_sta_names = igs_sta_obj.value("name");
  // /*if (igs_sta_names.type() == QJsonValue::Array) */
  //QJsonArray igs_names_arr = igs_sta_names.toArray();
  // igs_sta_obj.
  // int cnt = igs_sta_obj.count();
  for (int i = 0; i < igs_sta_obj.count(); ++i) {
    auto sta_name_obj = igs_sta_obj[i].toObject();
    auto sta_name = sta_name_obj.value("name").toString();
    qDebug() << sta_name_obj << "\n";
    ui->ComboSeleSta->addItem(sta_name);
    auto lat_lon_height = sta_name_obj.value("llh").toArray();
    qreal lat = lat_lon_height[0].toDouble();
    qreal lon = lat_lon_height[1].toDouble();
    qreal height = lat_lon_height[2].toDouble();
    this->igs_stations[sta_name] = (Stations(sta_name, lat, lon, height));
  }
}


QJsonDocument LoadIgsStationsFromJson(QString path)
{
  if (!QFile::exists(path)) {
    qDebug() << "No such file " + path;
  }
  QFile igs_file{path};
  igs_file.open(QIODevice::ReadOnly);
  qDebug() << igs_file.isOpen();
  QTextStream igs_file_stream(&igs_file);
  igs_file_stream.setEncoding(QStringConverter::Utf8);
  QString igs_str(igs_file_stream.readAll());
  QJsonParseError* json_err = new QJsonParseError();
  // auto aaaa = igs_str.toUtf8();
  QJsonDocument igs_stations_json = QJsonDocument::fromJson(igs_str.toUtf8(), json_err);
  // QJsonObject igs_sta_obj = igs_stations_json.object();
  // QJsonValue igs_sta_names = igs_sta_obj.value("name");

  return igs_stations_json;
}

void QGOPDD::on_BtnAddSta_clicked()
{
  static QMap<QString, bool> sta_added;
  QString sta_name = ui->ComboSeleSta->currentText();
  if (sta_added[sta_name]) {
    return;
  }
  sta_added[sta_name] = true;
  Stations sta = this->igs_stations[sta_name];
  qint64 curr_rows = ui->TableWidgetAddedStas->rowCount();
  ui->TableWidgetAddedStas->setRowCount(curr_rows + 1);
  QTableWidgetItem item;
  ui->TableWidgetAddedStas->setItem(curr_rows, 0, new QTableWidgetItem(sta_name));
  auto [name, lat, lon, h] = sta.GetInfo();
  ui->TableWidgetAddedStas->setItem(curr_rows, 1, new QTableWidgetItem(QString::number(lat)));
  ui->TableWidgetAddedStas->setItem(curr_rows, 2, new QTableWidgetItem(QString::number(lon)));
  ui->TableWidgetAddedStas->setItem(curr_rows, 3, new QTableWidgetItem(QString::number(h)));
  // ui->TableWidgetAddedStas.I
}


void QGOPDD::on_BtnEndCalender_clicked(bool checked)
{
  // CalendarWidget* calendar = new CalendarWidget();
  // calendar->show();
  this->endcalendar->show();

  // emit calendar->DateSelected()
  // ui->LineEndDoy->setText(QString::number(end.GetDayOfYear()));
  // ui->LineEndYear->setText(QString::number(end.GetYear()));
  // this->maincalender->show();
  // if (1 && !maincalendar->isActiveWindow()/*calendar->close()*/) {
  //   int year = calendar->year;
  //   int doy = calendar->doy;
  //   ui->LineEndYear->setText(QString::number(year));
  //   ui->LineEndDoy->setText(QString::number(doy));
  // }
  // int year = maincalender->GetYear();
  // int doy = maincalender->GetDayOfYear();
}

void QGOPDD::GetBeginDateSelected(QDate date) {
  int year = date.year(), doy = date.dayOfYear();
  ui->LineBeginYear->setText(QString::number(year));
  ui->LineBeginDoy->setText(QString::number(doy));
}

void QGOPDD::on_BtnBeginCalendar_clicked(bool checked)
{
  this->begincalender->show();
  // maincalender->show();
  // calendar
}

void QGOPDD::GetEndDateSelected(QDate date)
{
  int year = date.year(), doy = date.dayOfYear();
  // this->end.SetYear(year), this->end.SetDayOfYear(doy);
  this->ui->LineEndYear->setText(QString::number(year));
  this->ui->LineEndDoy->setText(QString::number(doy));
}

void QGOPDD::on_BtnGotoStation_clicked(bool check)
{
  if (1) {
  // igs_page->load(igs_page->url());
  QString sta_name = ui->ComboSeleSta->currentText();
  static int search_click_times = 1;
  // QWebEnginePage* igs_page = new QWebEnginePage(ui->IgsNetworkWebWidget);

  // ui->IgsNetworkWebWidget->loadFinished()
/*  qDebug() << QObject::connect(this->igs_page, &QWebEnginePage::loadFinished,
                   ui->IgsNetworkWebWidget, [this, sta_name](bool s) */{
    if (1) {
      qDebug() << igs_page->url();
      // if (!(search_click_times & 1)) {
      //   igs_page->load(igs_page->url());
      // }
      qDebug() << igs_page->isLoading();
      // ui->IgsNetworkWebWidget->setPage(igs_page);
      QString js_input_sta_name = "document.getElementById('id_search').value = '" + sta_name.mid(0, 2) + "'";
      // igs_page->runJavaScript(js_input_sta_name);
      js_input_sta_name = "document.getElementById('id_search').value = '" + sta_name + "'";
      // igs_page->runJavaScript("document.getElementById('id_search').value = 'KOUR'");
      QString js_click_adv_search = "var adv_search = document.querySelector('.no-filter-text').click();";
                                    // "adv_search;";
      // igs_page->runJavaScript(js_click_adv_search);
      // igs_page->runJavaScript("document.querySelector('.btn-close').click();");
      // igs_page->runJavaScript(js_click_adv_search);


                                       // "search_button.click();";

      qDebug() << "hello" << "\n";
      //
      // if (!(search_click_times & 1)) {
      //   igs_page->runJavaScript(js_click_search_button);
      //   igs_page->runJavaScript(js_click_search_button);
      // }
      search_click_times += 1;
      QString js_sta_board = "var sta_board = document.querySelector("
                                   "'.btn.btn-primary.sidebar-toggle.rounded-rect.right.text-nowrap');";
      QString js_click_command = "sta_board.click();";
      // igs_page->runJavaScript(js_sta_board);
      // igs_page->runJavaScript(js_click_search_button);
      // static int a = 1;
      igs_page->runJavaScript(js_input_sta_name); {
        igs_page->runJavaScript(
            "document.getElementById('id_search').dispatchEvent(new Event('change'));");
            // [=](const QVariant& res) {
              if (sta_name == "") return;
              QTimer* timer = new QTimer();
              connect(timer, &QTimer::timeout, this, [=](){
                QString sta_fly = "document.getElementById('" + sta_name
                                + "').querySelector('.btn.btn-primary.fly-btn').click();";
              // QThread::sleep(200);
              // igs_page->runJavaScript("document.querySelector('.btn.btn-primary.fly-btn').click()");
                igs_page->runJavaScript(sta_fly);
                // igs_page->runJavaScript("document.getElementById('id_search').value = '';");
                timer->stop();
              });
              timer->start(3000);
              return;
            // });
        // igs_page->runJavaScript("document.querySelector('.btn.btn-primary.fly-btn').click()");
        // QString js_click_search_button = "var search_button "
        //                                  "= document.querySelector('.btn.btn-success').click();";

	QString js_site_name = "var site = "
			       "document.querySelector("
			       "'.name.dt-center.dtfc-fixed-left.sorting_asc').click()";
	// igs_page->runJavaScript(js_sta_board+js_click_command);
	// igs_page->runJavaScript(js_site_name);
	// // igs_page->runJavaScript(js_click_search_button);
	// // igs_page->runJavaScript(js_click_adv_search);
	// igs_page->runJavaScript(js_click_search_button);
	// igs_page->runJavaScript(js_sta_board);
	// igs_page->runJavaScript(js_sta_board);

      }/*);*/

      // igs_page->runJavaScript("console.log()");
      // igs_page->runJavaScript(js_click_adv_search);
      // igs_page->runJavaScript(js_click_search_button);
      // igs_page->runJavaScript("document.querySelector('.btn-close').click();");
      // igs_page->runJavaScript(js_click_search_button);
      // igs_page->runJavaScript(js_click_command);
      // igs_page->runJavaScript(js_click_command);
      // igs_page->runJavaScript(keypress);
      // igs_page->runJavaScript(listen);
      // igs_page->load(QUrl("https://network.igs.org"));
    } else {
      qDebug() << "No!!!!!!!";
    }
  }
   // });
  // QString js_input_sta_name = "document.getElementById('id_search').value = '" + sta_name + "';";
  // igs_page->runJavaScript(js_input_sta_name);
  // QString js_click_search_button = "var search_button "
  //                                  "= document.querySelector('.btn.btn-success').click();";
  // igs_page->runJavaScript(js_click_search_button);
  }
}

