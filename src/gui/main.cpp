#include <QApplication>
#include <QDesktopWidget>

#include "hddsettings.h"
#include "hddcontroller.h"

int main(int argc, char* argv[]) {
//   Q_INIT_RESOURCE(qfi);
   QApplication app(argc, argv);
   // Desktop size
   int deskWidth = QApplication::desktop()->width();
   int deskHeight = QApplication::desktop()->height();
   
   HDDSettings* settings = new HDDSettings(QString());// replace default constructor if supplied by command line
   
   HDDController hddc(settings);

   return app.exec();
}
