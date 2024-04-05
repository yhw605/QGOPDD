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
  // QString CurrentFilename();
  // bool IsFileOpened();
  // void TextCut();
  // void TextCopy();
  // void TextPaste();

public slots:
  void setMyWindowModified() {
    // 由于textChanged信号没有参数，所以槽函数也不需要参数
    // 你可以在这里根据需要设置窗口标题或其他操作
    setWindowModified(true); // 假设文本被修改了
  }

private:
  Ui::TFormDoc *ui;
  QString filename_;
  bool file_opened_ = false;
};

#endif // TFORMDOC_H
