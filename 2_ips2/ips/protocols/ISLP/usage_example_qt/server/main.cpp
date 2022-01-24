#include "serverwindow.hpp"

#include <QApplication>

char mode=ST_MODE_STANDART;
int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  serverWindow w;
  w.show();
  return a.exec();
}
