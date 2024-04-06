#include "qgopdd.h"
#include "tformdoc.h"

#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QGOPDD w;
  w.show();
  // TFormDoc t;
  // t.LoadFromFile("");
  // t.show();
  return a.exec();
}
