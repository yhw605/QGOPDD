#include "qgopdd.h"

#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QGOPDD w;
  w.show();
  return a.exec();
}
