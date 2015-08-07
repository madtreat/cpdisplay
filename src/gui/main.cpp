#include <QApplication>
#include <QDesktopWidget>

#include "core/cpdsettings.h"
//#include "cpdcontroller.h"
#include "window.h"

int main(int argc, char* argv[]) {
   QApplication app(argc, argv);
   
   qDebug() << "Initializing...";
   CPDSettings* settings = new CPDSettings(QString());// replace default constructor if supplied by command line
   
   CPDWindow window(settings);
   window.show();//FullScreen();

   return app.exec();
}
