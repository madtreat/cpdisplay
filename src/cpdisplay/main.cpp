
#include <QCoreApplication>
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "core/cpdsettings.h"
//#include "cpdcontroller.h"
#include "window.h"

int main(int argc, char* argv[]) {
   QApplication app(argc, argv);
   QCoreApplication::setApplicationName("cpdisplay");
   QCoreApplication::setApplicationVersion("2.0.0");

   // Create the arg parser
   QCommandLineParser parser;
   parser.setApplicationDescription("Cockpit Display Help");
   parser.addHelpOption();
   parser.addVersionOption();

   // Verbose mode
   QCommandLineOption verboseOption({"v", "verbose"},
                                    QCoreApplication::translate("main", "Verbose mode.  Prints out more information."));
   parser.addOption(verboseOption);

   // User-specified configuration file
   QCommandLineOption configOption( {"c", "config"},
                                    QCoreApplication::translate("main", "Config file.  Use this <settings.ini> file instead of the default."),
                                    QCoreApplication::translate("main", "settings.ini"));
   parser.addOption(configOption);

   // Parse the command line args
   parser.process(app);

   qDebug() << "Initializing...";
   qDebug() << "Config option:" << parser.value(configOption);
   CPDSettings* settings = new CPDSettings(parser.value(configOption));// replace default constructor if supplied by command line
   
   CPDWindow window(settings);
   window.show();//FullScreen();

   return app.exec();
}
