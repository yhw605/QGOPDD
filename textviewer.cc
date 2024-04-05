#include "textviewer.h"
#include "ui_textviewer.h"
#include <QFileSystemModel>
#include <QMouseEvent>

TextViewer::TextViewer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TextViewer)
{
  ui->setupUi(this);
  ui->mdiArea->setTabsMovable(true);
  this->setWindowTitle("Rinex Viewer");
  // setCentralWidget(ui->mdiArea);
  setWindowState(Qt::WindowMaximized);
}

void TextViewer::SetDirname(QString dirname) {
  this->dirname_ = dirname;
  ui->lineEdit->setText(dirname);
  this->file_model_ = new QFileSystemModel(this);
  this->file_model_->setRootPath(dirname);
  qDebug() << this->file_model_->rootDirectory();

  ui->treeView->setModel(this->file_model_);
  ui->treeView->setRootIndex(this->file_model_->index(dirname));
  // ui->treeView->show();
  qDebug() << dirname;
}

TextViewer::~TextViewer()
{
  delete ui;
}
