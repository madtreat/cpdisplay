
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
      {"i", "d-dref-id"},
      QCoreApplication::translate("main", "Print DREF ID's.")
    },
    {
      {"s", "d-send"},
      QCoreApplication::translate("main", "Verbosely write to the network.")
    },
    {
      {"u", "d-recv-udp"},
      QCoreApplication::translate("main", "Verbosely receive UDP.")
    },
    {
      {"p", "d-recv-packet"},
      QCoreApplication::translate("main", "Verbosely receive packets.")
    },
    {
      {"r", "d-recv-rref"},
      QCoreApplication::translate("main", "Verbosely receive RREF data.")
    },
    {
      {"d", "d-recv-ds"},
      QCoreApplication::translate("main", "Verbosely receive data from the DataSwitch.")
    },
    {
      {"e", "d-settings"},
      QCoreApplication::translate("main", "Verbosely read settings.")
    },
    {
      {"f", "d-forward"},
      QCoreApplication::translate("main", "Verbosely forward data packets. Only applies to the MCS Data Switch")
    }
  });

  // Parse the command line args
  parser.process(app);

  uint16_t dfi = 0;
  dfi |= parser.isSet("d-dref-id")      ? DEBUG_DREF_ID     : 0;
  dfi |= parser.isSet("d-send")         ? DEBUG_SEND        : 0;
  dfi |= parser.isSet("d-recv-udp")     ? DEBUG_RECV_UDP    : 0;
  dfi |= parser.isSet("d-recv-packet")  ? DEBUG_RECV_PACKET : 0;
  dfi |= parser.isSet("d-recv-rref")    ? DEBUG_RECV_RREF   : 0;
  dfi |= parser.isSet("d-recv-ds")      ? DEBUG_RECV_DATASWITCH : 0;
  dfi |= parser.isSet("d-settings")     ? DEBUG_SETTINGS    : 0;
  dfi |= parser.isSet("d-forward")      ? DEBUG_FORWARD     : 0;

  qDebug() << "Initializing...";
  qDebug() << "Config option:" << parser.value("config");
  CPDSettings* settings = new CPDSettings(parser.value("config"), (DebugType) dfi);

  CPDWindow window(settings);
  window.show();//FullScreen();

  return app.exec();
}
