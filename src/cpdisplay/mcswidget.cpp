#include "mcswidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "mcsslavewidget.h"
#include "core/cpdsettings.h"


MCSWidget::MCSWidget(CPDSettings* _settings, QList<SwitchBoard*> _sbs, QFrame* _parent)
: QFrame(_parent),
  settings(_settings),
  slaveSwitches(_sbs)
{
   setObjectName("include-background");
   QVBoxLayout* layout = new QVBoxLayout(this);

   QLayout* labelsLayout = createSlaveWidgetLabels();

   QHBoxLayout* slaveWidgetLayout = new QHBoxLayout();
   slaveWidgetLayout->addLayout(labelsLayout);
   for (int i = 0; i < settings->numSlaves(); ++i)
   {
      MCSSlaveWidget* slave = new MCSSlaveWidget(settings, slaveSwitches.at(i), i);
      slaveWidgets.append(slave);
      slaveWidgetLayout->addWidget(slave);
   }
   layout->addLayout(slaveWidgetLayout);
}

MCSWidget::~MCSWidget()
{
}


QLayout* MCSWidget::createSlaveWidgetLabels()
{
   QVBoxLayout* layout = new QVBoxLayout(this);

   name = new QLabel("Station:");
   layout->addWidget(name);

   pause = new QPushButton("Resume All");
   pause->setCheckable(true);
   pause->setChecked(true);
   connect(pause, &QPushButton::toggled, this, &MCSWidget::pauseAllSimulators);
   layout->addWidget(pause);

   status = new QLabel("Connection Status:");
   layout->addWidget(status);

   ipSim = new QLabel("Simulator IP:");
   layout->addWidget(ipSim);

   ipCPD = new QLabel("CPDisplay IP:");
   layout->addWidget(ipCPD);

   aircraft = new QLabel("Aircraft:");
   layout->addWidget(aircraft);

   flaps = new QLabel("Flaps (%)");
   layout->addWidget(flaps);

   gearStatus = new QLabel("Landing Gear:");
   layout->addWidget(gearStatus);

   altitude = new QLabel("Altitude (FSL):");
   layout->addWidget(altitude);

   airspeed = new QLabel("Airspeed (KIAS):");
   layout->addWidget(airspeed);

   mach = new QLabel("Mach-o-Meter:");
   layout->addWidget(mach);

   heading = new QLabel("Heading:");
   layout->addWidget(heading);

   fuel = new QLabel("Fuel (%):");
   layout->addWidget(fuel);

   timeLocal = new QLabel("Local Time (24 hour):");
   layout->addWidget(timeLocal);

   timeZulu = new QLabel("Zulu Time (24 hour):");
   layout->addWidget(timeZulu);

   wheelBreaks = new QLabel("Parking Break:");
   layout->addWidget(wheelBreaks);

   return layout;
}

void MCSWidget::pauseAllSimulators(bool paused)
{
   qDebug() << "Pausing all simulators.";
   pause->setText(paused ? "Resume All" : "Pause All");
   foreach (MCSSlaveWidget* slave, slaveWidgets)
   {
      slave->pauseSimulator(paused);
   }
}
