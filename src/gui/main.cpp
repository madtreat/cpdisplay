#include <QApplication>
#include <QDesktopWidget>

#include "core/hddsettings.h"
//#include "hddcontroller.h"
#include "window.h"

int main(int argc, char* argv[]) {
   QApplication app(argc, argv);
   
   qDebug() << "Initializing...";
   HDDSettings* settings = new HDDSettings(QString());// replace default constructor if supplied by command line
   
   HDDWindow window(settings);
   window.show();//FullScreen();

   return app.exec();
}
