#include "textviewer.h"
#include "ui_textviewer.h"

TextViewer::TextViewer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TextViewer)
{
  ui->setupUi(this);
  ui->mdiArea->setTabsMovable(true);
  setCentralWidget(ui->mdiArea);
  setWindowState(Qt::WindowMaximized);
}

TextViewer::~TextViewer()
{
  delete ui;
}
