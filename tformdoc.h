#ifndef TFORMDOC_H
#define TFORMDOC_H

#include <QWidget>

namespace Ui {
class TFormDoc;
}

class TFormDoc : public QWidget
{
  Q_OBJECT

public:
  explicit TFormDoc(QWidget *parent = nullptr);
  ~TFormDoc();
  void LoadFromFile(QString filename);
  void Save2File();
  QString CurrentFilename();
  bool IsFileOpened();
  void TextCut();
  void TextCopy();
  void TextPaste();

private:
  Ui::TFormDoc *ui;
  QString filename_;
  bool file_opened_ = false;
};

#endif // TFORMDOC_H
