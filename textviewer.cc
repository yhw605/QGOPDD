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
  ui->treeView->show();
  // ui->treeView->show();
  qDebug() << dirname;
}

TextViewer::~TextViewer()
{
  delete ui;
}

void TextViewer::on_treeView_doubleClicked(const QModelIndex &index)
{
  if (index.column() == 0) {
    TFormDoc* t = new TFormDoc();
    auto ind = index;
    QString data;
    while (true) {
      auto da = ind.data();
      auto pa = ind.parent();
      // if (dirname_.contains((pa.data().toString()))) {
      //   break;
      // }
      data = "/" + da.toString() + data;
      ind = pa;
      if (dirname_.contains((pa.data().toString()))) {
        break;
      }
    }
    qDebug() << data;
    qDebug() << this->dirname_ + data;
    t->LoadFromFile(this->dirname_ + "/" + data);
    ui->mdiArea->addSubWindow(t);
    t->show();
  }
}

