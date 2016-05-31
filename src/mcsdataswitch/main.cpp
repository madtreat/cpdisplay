#include <QCoreApplication>
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "core/cpdsettings.h"
#include "mcsdataswitch.h"


int main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  QCoreApplication::setApplicationName("cpdataswitch");
  QCoreApplication::setApplicationVersion("1.0.0");

  QCommandLineParser parser;
  parser.setApplicationDescription("cpdataswitch Help");
  parser.addHelpOption();
  parser.addVersionOption();

  parser.addOptions({
    {
      {"v", "verbose"},
      QCoreApplication::translate("main", "Print verbose output")
    },
    {
      {"c", "config"},
      QCoreApplication::translate("main", "Config file.  Use this <mcs-settings.ini> file instead of the default."),
      QCoreApplication::translate("main", "settingsfile")
    }
  });
  parser.process(app);

  // replace default constructor if supplied by command line
  CPDSettings* settings = new CPDSettings(parser.value("settingsfile"));

  if (!settings->isMCS()) {
    qWarning() << "Warning: MCS is false, please check your settings.ini file.";
    return 1;
  }

  MCSDataSwitch ds(settings);

  return app.exec();
}

