#include "qgopdd.h"
#include "./ui_qgopdd.h"
#include "calendarwidget.h"
#include "ftpdownloader.h"

#include <QWebChannel>
#include <QWebEnginePage>
#include <QTimer>
// #include "curl/curl.h"
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
  // d.DownloadFtp("");
  // d.DownloadGnssObs("c:/users/yhw60/desktop",
  //                   "ftp://igs.gnsswhu.cn/pub/gps/data/daily/2022/001/22d/", "ABMF");
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
  this->endcalendar->show();
}

void QGOPDD::GetBeginDateSelected(QDate date) {
  int year = date.year(), doy = date.dayOfYear();
  this->begin.SetDate(date);
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
  this->end.SetDate(date);
  // this->end.SetYear(year), this->end.SetDayOfYear(doy);
  this->ui->LineEndYear->setText(QString::number(year));
  this->ui->LineEndDoy->setText(QString::number(doy));
}

void QGOPDD::on_BtnGotoStation_clicked(bool check)
{
  // igs_page->load(igs_page->url());
  QString sta_name = ui->ComboSeleSta->currentText();
  // static int search_click_times = 1;
  // QWebEnginePage* igs_page = new QWebEnginePage(ui->IgsNetworkWebWidget);

  // ui->IgsNetworkWebWidget->loadFinished()
  /*  qDebug() << QObject::connect(this->igs_page, &QWebEnginePage::loadFinished,
                   ui->IgsNetworkWebWidget, [this, sta_name](bool s) */
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
  // search_click_times += 1;
  QString js_sta_board = "var sta_board = document.querySelector("
                         "'.btn.btn-primary.sidebar-toggle.rounded-rect.right.text-nowrap');";
  QString js_click_command = "sta_board.click();";
  // igs_page->runJavaScript(js_sta_board);
  // igs_page->runJavaScript(js_click_search_button);
  // static int a = 1;
  igs_page->runJavaScript(js_input_sta_name); {
    igs_page->runJavaScript(
        "document.getElementById('id_search').dispatchEvent(new Event('change'));",
        [this, sta_name](const QVariant& res) {
          if (sta_name == "") return;
          qDebug() << this->igs_page->isLoading();
          QTimer* timer = new QTimer();
          connect(timer, &QTimer::timeout, this, [this, timer, sta_name](){
            QString sta_fly = "document.getElementById('" + sta_name
                              + "').querySelector('.btn.btn-primary.fly-btn').click();";
            igs_page->runJavaScript(sta_fly);
            // igs_page->runJavaScript("document.getElementById('id_search').value = '';");
            timer->stop();
          });
          qDebug() << igs_page->isLoading();
          timer->start(4000);

	  // return;
	});
    // QString js_site_name = "var site = "
    //                        "document.querySelector("
    //                        "'.name.dt-center.dtfc-fixed-left.sorting_asc').click()";
  }
}

QStringList QGOPDD::GetWaitingDownloadList() {
  QStringList stations;
  int rows = ui->TableWidgetAddedStas->rowCount();
  QTableWidget* table = ui->TableWidgetAddedStas;
  for (int i = 0; i < rows; ++i) {
    stations.push_back(table->item(i, 0)->text());
  }
  return stations;
}



void QGOPDD::on_BtnStartDownload_clicked() {
  // ui->statusBar->show();
  QLabel *plabel = new QLabel();
  plabel->setText("Start downloading.");
  // progress_bar->setRange(0, 100);
  // progress_bar->setValue(10);
  this->waiting_list = GetWaitingDownloadList();
  // QDate start;start.setDate()
  auto curr_date = this->begin.GetDate(), end_date = this->end.GetDate();
  if (curr_date.isNull()) {
    QWidget* w = new QWidget();
    w->setMaximumSize(360, 180);
    w->setMinimumSize(120, 60);
    QLabel* label = new QLabel(w);
    label->setMinimumSize(100, 50);
    label->setMaximumSize(300, 150);
    // label
    QVBoxLayout* layout = new QVBoxLayout(w);
    label->setText("You haven't select the date!");
    layout->addWidget(label);
    w->setLayout(layout);
    w->show();
    QTimer* timer = new QTimer();
    connect(timer, &QTimer::timeout, w, [=](){
      w->close();
      timer->stop();
    });
    timer->start(5000);
    // delete progress_bar;
    // progress_bar = nullptr;
    return;
  }
  // QString ftp_src_dir = "ftp://igs.gnsswhu.cn/pub/gps/data/daily/";
  // this->progress_caller->SetProgress(0);
  // this->progress_caller->show();
  QProgressBar* progress_bar = new QProgressBar();
  ui->statusBar->addWidget(progress_bar);
  int daydiff = end_date.toJulianDay() - curr_date.toJulianDay() + 1;
  int sta_size = ui->TableWidgetAddedStas->rowCount();
  progress_bar->setRange(0, daydiff * sta_size);
  // progress_bar->setValue(0);
  // progress_bar->show();
  while (curr_date <= end_date) {
    int doy = curr_date.dayOfYear();
    int year = curr_date.year();
    QString syear = QString::number(year).mid(2, 2);

    QString url = "ftp://igs.gnsswhu.cn/pub/gps/data/daily/" + QString::number(year)
                  + "/" + QString::number(doy).rightJustified(3, '0') + "/" + syear + "d/";
    this->ftp_downloader->FetchFileList(url);
    QString path = this->GetWd() + "/" + QString::number(doy).rightJustified(3, '0');
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
      progress_bar->setFormat("Date: " + curr_date.toString(Qt::ISODate)
                              + ",Downloading station: " + sta + " %v/%m");
      progress_bar->setValue(progress * 100);
      progress_bar->update();
    }
    this->ftp_downloader->ClearFilelist();
    url[url.size() - 2] = 'p';
    this->ftp_downloader->FetchFileList(url, "SHA512SUMS-NAV");
    if (ui->CheckBoxBrdm->isChecked()) {
      this->ftp_downloader->DownloadGnssObs(path, url, "BRDM");
    }
    if (ui->CheckBoxBrd4->isChecked()) {
      this->ftp_downloader->DownloadGnssObs(path, url, "BRD4");
    }
    curr_date = curr_date.addDays(1);
    this->ftp_downloader->ClearFilelist();
    // QProcess::start("./thirdparty/hatanaka/rnxcmp/source/CRX2RNX", {}) ;
  }
  progress_bar->close();
}


void QGOPDD::on_BtnDownloadDir_clicked()
{
  QString ini_path = "";
  QString dialog_title = "Select download directory.";
  // QFileDialog* dialog = new QFileDialog(this);
  QString workdir = QFileDialog::getExistingDirectory();
  this->SetWd(workdir);
  this->ftp_downloader->SetWd(workdir);
  ui->LineDownloadDir->setText(workdir);
  // dialog->setFileMode(QFileDialog::Director);
  // dialog.
}


void QGOPDD::on_CheckBoxBrdm_stateChanged(int state)
{
  if (state == Qt::Checked) {
    this->download_type_["BRDM"] = true;
  } else {
    this->download_type_["BRDM"] = false;
  }
}


void QGOPDD::on_CheckBoxBrd4_stateChanged(int state)
{
  if (state == Qt::Checked) {
    this->download_type_["BRD4"] = true;
  } else {
    this->download_type_["BRD4"] = false;
  }
}


void QGOPDD::on_CheckBoxSp3_stateChanged(int state)
{
  if (state == Qt::Checked) {
    this->download_type_["SP3"] = true;
  } else {
    this->download_type_["SP3"] = false;
  }
}

