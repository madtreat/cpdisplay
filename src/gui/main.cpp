#include <QApplication>
#include <QDesktopWidget>
#include "window.h"
//#include "MainWindow.h"

int main(int argc, char* argv[]) {
//   Q_INIT_RESOURCE(qfi);
   QApplication app(argc, argv);
   // Desktop size
   int deskWidth = QApplication::desktop()->width();
   int deskHeight = QApplication::desktop()->height();
   
   HDDWindow window;
//   // Window size
//   int windowWidth = window.width();
//   int windowHeight = window.height();
//   MainWindow window;

//   window.showMaximized();
   window.show();

   return app.exec();
}
