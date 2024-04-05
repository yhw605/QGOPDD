#include "tformdoc.h"
#include "ui_tformdoc.h"
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QSaveFile>

TFormDoc::TFormDoc(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TFormDoc)
{
  ui->setupUi(this);
  this->setWindowTitle("Unnamed.txt[*]");
  // connect(ui->textEdit, &QTextEdit::textChanged, this, &QWidget::setWindowModified);
}

TFormDoc::~TFormDoc()
{
  delete ui;
}

void TFormDoc::LoadFromFile(QString filename) {
  QFile txtfile(filename);
  if (txtfile.open(QIODevice::ReadWrite | QIODevice::Text)) {
    QTextStream txt_stream(&txtfile);
    ui->textEdit->clear();
    ui->textEdit->setPlainText(txt_stream.readAll());
    txtfile.close();

    this->filename_ = filename;
    QFileInfo file_info(filename);
    this->setWindowTitle(file_info.fileName());
    this->file_opened_ = true;
  }
}

void TFormDoc::Save2File() {

  // this->setWindowModified(false);
}
