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
#include <QProcess>

constexpr int LONG_RNX_OBS_NAME_LEN_GZ = 41;
constexpr int LONG_RNX_NAV_NAME_LEN_GZ = 37;
constexpr int SHORT_RNX_NAME_LEN_GZ = 15;

#include <curl/curl.h>

FtpDownloader::FtpDownloader(/*QObject*/QThread *parent)
    : /*QObject*/QThread{parent} {
  // connect()
}

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

void FtpDownloader::FetchFileList(QString ftp_addr, QString filename, QString username, QString password) {
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
  if (filename == "") {
  } else {
    filelist.filename = this->GetWd() + "/" + sdoy + "/" + filename;
  }
  // curl = curl_easy_init();

  DownloadFtpFile(curl, filelist, ftp_addr);

  QFile read_file_list(filelist.filename);
  read_file_list.open(QIODevice::ReadOnly);
  QTextStream filelist_stream{read_file_list.readAll()};
  qDebug() << read_file_list.isOpen();
  while (!filelist_stream.atEnd()) {
    QString line = filelist_stream.readLine();
    QStringList line_list = line.split(' ');
    for (int i = 0; i < line_list.size(); ++i) {
      int len = line_list[i].size();
      switch (len) {
        case LONG_RNX_OBS_NAME_LEN_GZ:
        case LONG_RNX_NAV_NAME_LEN_GZ:
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

  auto file_list = this->GetFileList();

  QString filename = GetRinexFilename(sta_name.mid(0, 4), file_list);
  if (filename.isEmpty()) {
    return;
  }
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

    qDebug() << "Filename: " + QString(file.filename);
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      qDebug() << "Curl meets error " + QString(curl_easy_strerror(res));
      return;
    }
  }
  curl_global_cleanup();
  fclose(file.stream);
  emit ProgressChanged(progress_++);
  // std::filesystem::path directory{path.toStdString()};
  // std::filesystem::path sys_filename{filename.toStdString()};
  // std::filesystem::path fullpath = directory / sta_name.toStdString();
}

void FtpDownloader::ExtractGzFile(QString path, QString sta_name) {
  QDir dir(path);
  if (!dir.exists()) {
    qDebug() << "The file is not exsit.";
    return;
  }
  QStringList filters;
  filters<< "*.crx.gz"  << "*.rnx.gz" << "*.sp3.gz" << "*.eph.gz" << "*.*o.gz" << "*.*p.gz" << "*.*n.gz";
  // filters.append("*.rnx.gz"), filters.append("*.sp3.gz");
  // filters.append("*.eph.gz"), filters.append("*.*o.gz");
  // filters.append("*.*n.gz"), filters.append("*.*p.gz");
  QDir::Filters filter_flags = QDir::Files | QDir::NoSymLinks;
  QDir::SortFlags sort_flags = QDir::Name;
  QFileInfoList entries = dir.entryInfoList(filters, filter_flags, sort_flags);

  for (auto& entry : entries) {
    QString filename = entry.absoluteFilePath();
    // QString fileName = entry.fileName();
    qDebug() << "Found file:" << filename;
    QStringList args_gz;
    args_gz << "-d" << filename;
    QProcess* process = new QProcess();
    process->start(MINIGZIP_PATH, args_gz);
  }
}
