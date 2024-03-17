#include "progresscaller.h"
#include "ui_progresscaller.h"

ProgressCaller::ProgressCaller(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ProgressCaller)
{
  ui->setupUi(this);
}

ProgressCaller::~ProgressCaller()
{
  delete ui;
}

void ProgressCaller::SetProgress(int progress) {
  ui->ProgressBarDownload->setValue(progress);
}
