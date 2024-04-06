#include "downloadfinished.h"
#include "ui_downloadfinished.h"

DownloadFinished::DownloadFinished(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DownloadFinished)
{
  ui->setupUi(this);
  this->setWindowTitle("");
}

DownloadFinished::~DownloadFinished()
{
  delete ui;
}
