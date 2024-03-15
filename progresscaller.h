#ifndef PROGRESSCALLER_H
#define PROGRESSCALLER_H

#include <QWidget>
#include <QProgressBar>
#include <QThread>

namespace Ui {
class ProgressCaller;
}

class ProgressCaller : public QWidget
{
  Q_OBJECT

public:
  explicit ProgressCaller(QWidget *parent = nullptr);
  ~ProgressCaller();

private:
  Ui::ProgressCaller *ui;
};

#endif // PROGRESSCALLER_H
