#include "qgopdd.h"
// #include "tformdoc.h"

#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QGOPDD w;
  QIcon icon(":/img/QGOPDD.png");
  // w.setWindowIcon(icon);
  w.show();
  // TFormDoc t;
  // t.LoadFromFile("");
  // t.show();
  return a.exec();
}
