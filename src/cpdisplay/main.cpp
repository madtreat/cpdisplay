
#include <QCoreApplication>
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "core/cpdsettings.h"
//#include "cpdcontroller.h"
#include "cpdwindow.h"

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
  parser.addOptions({
    // {
    //   {"v", "verbose"},
    //   QCoreApplication::translate("main", "Verbose mode")
    // },
    {
      {"c", "config"},
      QCoreApplication::translate("main", "Config file.  Use this <settings.ini> file instead of the default."),
      QCoreApplication::translate("main", "settings.ini")
    },
    {
      {"i", "dref-id"},
      QCoreApplication::translate("main", "Print DREF ID's.")
    },
    {
      {"s", "send"},
      QCoreApplication::translate("main", "Verbosely write to the network.")
    },
    {
      {"u", "recv-udp"},
      QCoreApplication::translate("main", "Verbosely receive UDP.")
    },
    {
      {"p", "recv-packet"},
      QCoreApplication::translate("main", "Verbosely receive packets.")
    },
    {
      {"r", "recv-rref"},
      QCoreApplication::translate("main", "Verbosely receive RREF data.")
    },
    {
      {"d", "recv-ds"},
      QCoreApplication::translate("main", "Verbosely receive data from the DataSwitch.")
    },
    {
      {"c", "settings"},
      QCoreApplication::translate("main", "Verbosely read settings.")
    },
    {
      {"f", "forward"},
      QCoreApplication::translate("main", "Verbosely forward data packets. Only applies to the MCS Data Switch")
    }
  });

  // Parse the command line args
  parser.process(app);

  qDebug() << "Initializing...";
  qDebug() << "Config option:" << parser.value("config");
  CPDSettings* settings = new CPDSettings(parser.value("config"));

  CPDWindow window(settings);
  window.show();//FullScreen();

  return app.exec();
}
