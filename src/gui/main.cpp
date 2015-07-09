#include <QApplication>
#include <QDesktopWidget>

#include "core/hddsettings.h"
//#include "hddcontroller.h"
#include "window.h"

int main(int argc, char* argv[]) {
//   Q_INIT_RESOURCE(qfi);
   QApplication app(argc, argv);
   // Desktop size
   int deskWidth = QApplication::desktop()->width();
   int deskHeight = QApplication::desktop()->height();
   
   qDebug() << "Initializing...";
   HDDSettings* settings = new HDDSettings(QString());// replace default constructor if supplied by command line
   
//   HDDController hddc(settings);
   HDDWindow window(settings);
   window.showFullScreen();

   return app.exec();
}
