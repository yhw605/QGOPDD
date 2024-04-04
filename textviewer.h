#ifndef TEXTVIEWER_H
#define TEXTVIEWER_H

#include <QMainWindow>

namespace Ui {
class TextViewer;
}

class TextViewer : public QMainWindow
{
  Q_OBJECT

public:
  explicit TextViewer(QWidget *parent = nullptr);
  ~TextViewer();

private:
  Ui::TextViewer *ui;
};

#endif // TEXTVIEWER_H
