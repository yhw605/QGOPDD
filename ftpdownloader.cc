#include "ftpdownloader.h"
// #include "curl/curl.h"
#include <QEventLoop>
#include <cstdio>
#include <QDir>
#include <filesystem>
#include <string>
#include <cstring>
// #include <memory>
#include <QRegularExpression>
#include <format>
#include <QTime>

constexpr int LONG_RNX_NAME_LEN_GZ = 41;
constexpr int SHORT_RNX_NAME_LEN_GZ = 15;

#include <curl/curl.h>

FtpDownloader::FtpDownloader(QObject *parent)
    : QObject{parent}
{}

struct FtpFile {
  QString filename = "";
  FILE* stream = nullptr;
};

// class FtpPath

static size_t ftp_fwrite(void* buffer, size_t size, size_t nmemb, void* stream) {
  struct FtpFile* out = (struct FtpFile*)stream;
  if (!out->stream) {
    if (fopen_s(&out->stream, out->filename.toStdString().c_str(), "wb") != 0) return 0; // Cannot open the file
  }
  return fwrite(buffer, size, nmemb, out->stream);
}

void DownloadFtpFile(CURL* curl, FtpFile file, QString ftp_addr, QString username = "", QString password = "") {
  if (curl) {
    char* ch_ftp_addr = new char[200];
    strcpy_s(ch_ftp_addr, 199, ftp_addr.toStdString().c_str());
    curl_easy_setopt(curl, CURLOPT_URL, ch_ftp_addr);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ftp_fwrite);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      qDebug() << "Curl meets error " + QString(curl_easy_strerror(res));
    }
  }
  if (file.stream) {
    std::fclose(file.stream);
  }
}

QString GetRinexFilename(QString sta_name, QStringList file_list) {
  for (auto file : file_list) {
    if (file.contains(sta_name)) {
      return file;
    }
  }
  return "";
}

void FtpDownloader::FetchFileList(QString ftp_addr, QString username, QString password) {
  curl_global_init(CURL_GLOBAL_DEFAULT);
  // std::shared_ptr<CURL> curl = std::make_shared<CURL>((CURL*)curl_easy_init());
  CURL* curl = curl_easy_init();
  QStringList* tmp = new QStringList(ftp_addr.split('/'));
  QString sdoy = tmp->at(tmp->size() - 3);
  QString str_curr_dir = this->GetWd() + "/" + sdoy;
  QDir curr_dir(str_curr_dir);
  if (!curr_dir.exists()) {
    curr_dir.mkdir(str_curr_dir);
  }
  // curl.reset(curl_easy_init());
  FtpFile filelist{this->GetWd() + "/" + sdoy + "/SHA512SUMS", nullptr};

  // curl = curl_easy_init();

  DownloadFtpFile(curl, filelist, ftp_addr);

  QFile read_file_list(this->GetWd() + "/" + sdoy + "/SHA512SUMS");
  read_file_list.open(QIODevice::ReadOnly);
  QTextStream filelist_stream{read_file_list.readAll()};
  qDebug() << read_file_list.isOpen();
  while (!filelist_stream.atEnd()) {
    QString line = filelist_stream.readLine();
    QStringList line_list = line.split(' ');
    for (int i = 0; i < line_list.size(); ++i) {
      int len = line_list[i].size();
      switch (len) {
        case LONG_RNX_NAME_LEN_GZ:
        case SHORT_RNX_NAME_LEN_GZ:
          file_list_.push_back(line_list[i]);
      }
    }
    // file_list_.push_back(line_list.back());
  }

  curl_easy_cleanup(curl);
}

void FtpDownloader::DownloadGnssObs(QString path, QString url, QString sta_name,
                                QString username, QString password) {
  // FetchFileList(url);

  CURL* curl;
  qDebug() << url;
  // first: download the filename list
  // struct FtpFile filelist = {
  //     "SHA512SUMS",
  //     NULL,
  // };
  auto file_list = this->GetFileList();

  QString filename = GetRinexFilename(sta_name.mid(0, 4), file_list);
  QString path_to_file = path + "/" + filename;

  FtpFile file{path_to_file, nullptr};
  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();

  if (curl) {
    char* fullname = new char[250];
    strcpy_s(fullname, 249, (url.toStdString() + filename.toStdString()).c_str());
    int ret = curl_easy_setopt(curl, CURLOPT_URL, fullname);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ftp_fwrite);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);
    // ret |= curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 100L);
    struct Progress {
      char *my_private;
      size_t size;
    };

    // ret |= curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, [](void *clientp,
    //                                                            curl_off_t dltotal,
    //                                                            curl_off_t dlnow,
    //                                                            curl_off_t ultotal,
    //                                                            curl_off_t ulnow){
    //   // Progress* memory = (Progress*)clientp;
    //   CURL* curl = static_cast<CURL*>(clientp);
    //   // qDebug() << std::format("my ptr: {}\n", memory->my_private);
    //   double speed = .0;
    //   QString unit = "B";

    //   curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD, &speed);
    //   double left_total = static_cast<double>(dltotal - dlnow);
    //   double left_time = .0;
    //   if (std::fabs(speed) < 1e-5) {
    //     left_time = (left_total) / speed;
    //     int hours = left_time / 3600;
    //     int minutes = (left_time - hours * 3600) / 60;
    //     int seconds = left_time - hours * 3600 - minutes * 60;
    //     qDebug() << std::format("Left time: {}h{}m{}s", hours, minutes, seconds);
    //   }

    //   return 0;
    // });
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      qDebug() << "Curl meets error " + QString(curl_easy_strerror(res));
    }
  }
  curl_global_cleanup();
  fclose(file.stream);
  std::filesystem::path directory{path.toStdString()};
  // std::filesystem::path sys_filename{filename.toStdString()};
  std::filesystem::path fullpath = directory / sta_name.toStdString();
}
