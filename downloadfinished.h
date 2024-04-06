#ifndef DOWNLOADFINISHED_H
#define DOWNLOADFINISHED_H

#include <QWidget>

namespace Ui {
class DownloadFinished;
}

class DownloadFinished : public QWidget
{
  Q_OBJECT

public:
  explicit DownloadFinished(QWidget *parent = nullptr);
  ~DownloadFinished();

private:
  Ui::DownloadFinished *ui;
};

#endif // DOWNLOADFINISHED_H
