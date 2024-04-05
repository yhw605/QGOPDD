#ifndef TEXTVIEWER_H
#define TEXTVIEWER_H

#include <QMainWindow>
#include <QFileSystemModel>

namespace Ui {
class TextViewer;
}

class TextViewer : public QMainWindow
{
  Q_OBJECT

public:
  explicit TextViewer(QWidget *parent = nullptr);
  ~TextViewer();
  void SetDirname(QString dirname);
private:
  Ui::TextViewer *ui;
  QString dirname_;
  QFileSystemModel* file_model_;
};

#endif // TEXTVIEWER_H
