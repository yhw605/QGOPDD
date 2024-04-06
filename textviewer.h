#ifndef TEXTVIEWER_H
#define TEXTVIEWER_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QVector>
#include "tformdoc.h"
// #include "downloadfinished.h"

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
private slots:
  void on_treeView_doubleClicked(const QModelIndex &index);

private:
  Ui::TextViewer *ui;
  QString dirname_;
  QFileSystemModel* file_model_;
  QVector<TFormDoc> doc_list_;
  // DownloadFinished* finished_widget_ = new DownloadFinished();
};

#endif // TEXTVIEWER_H
