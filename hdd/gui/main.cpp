#include <QApplication>
#include <QDesktopWidget>
#include "window.h"

int main(int argc, char* argv[]) {
   QApplication app(argc, argv);
   // Desktop size
   int deskWidth = QApplication::desktop()->width();
   int deskHeight = QApplication::desktop()->height();
   
   HDDWindow window;
   // Window size
   int windowWidth = window.width();
   int windowHeight = window.height();


   window.showMaximized();

   return app.exec();
}
