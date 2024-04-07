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
  connect(ui->textEdit, &QTextEdit::textChanged, this, &TFormDoc::setMyWindowModified);
  ui->textEdit->setFontFamily("Consolas");
  ui->textEdit->setFontPointSize(20);
  ui->textEdit->setLineWrapMode(QTextEdit::NoWrap);
}

TFormDoc::~TFormDoc()
{
  delete ui;
}

void TFormDoc::LoadFromFile(QString filename) {
  QFile txtfile(filename);
  if (txtfile.open(QIODevice::ReadWrite | QIODevice::Text)) {
    QTextStream txt_stream(&txtfile);
    // auto fptr = txtfile.map(0, txtfile.size() / 4);
    ui->textEdit->clear();
    ui->textEdit->setPlainText(txtfile.readAll());
    // const int chunkSize = 1024; // 例如，每次读取 1 KB
    // QByteArray buffer(chunkSize, '\0');

    // // 分块读取文件
    // while (!txt_stream.atEnd()) {
    //   auto buf = txt_stream.read(chunkSize);
    //   // txt_stream.read.readRawData(buffer.data(), chunkSize);
    //   ui->textEdit->append(buf);
    //   qApp->processEvents(); // 确保界面可以响应
    // }

    // // file.close();
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
