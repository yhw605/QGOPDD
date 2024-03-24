#include "progresscaller.h"
#include "ui_progresscaller.h"

ProgressCaller::ProgressCaller(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ProgressCaller)
{
  ui->setupUi(this);
  // ui->ProgressBarDownload->setRange(0, 100);
}

ProgressCaller::~ProgressCaller()
{
  delete ui;
}

void ProgressCaller::SetProgress(int progress) {
  ui->ProgressBarDownload->setValue(progress);
}

void ProgressCaller::RefreshWindow() {
  ui->ProgressBarDownload->update();
  ui->ProgressBarDownload->show();
}

void ProgressCaller::UpdateProgress(int progress) {
  SetProgress(progress);
}
