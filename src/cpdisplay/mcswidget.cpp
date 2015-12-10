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
   QVBoxLayout* layout = new QVBoxLayout();
   layout->setContentsMargins(0, 0, 0, 0);

   name        = new QLabel("Station:");
   tail        = new QLabel("Tail Number:");
   status      = new QLabel("Connection Status:");
   ipSim       = new QLabel("Simulator IP:");
   ipCPD       = new QLabel("CPDisplay IP:");
   aircraft    = new QLabel("Aircraft:");
   flaps       = new QLabel("Flaps (%)");
   altitude    = new QLabel("Altitude (FSL):");
   airspeed    = new QLabel("Airspeed (KIAS):");
   mach        = new QLabel("Mach-o-Meter:");
   heading     = new QLabel("Heading:");
   fuel        = new QLabel("Fuel (%):");
   timeLocal   = new QLabel("Local Time (24 hour):");
   timeZulu    = new QLabel("Zulu Time (24 hour):");

   pause = new QPushButton("All Paused");
   pause->setCheckable(true);
   pause->setChecked(true);
   connect(pause, &QPushButton::toggled, this, &MCSWidget::pauseAllSimulators);
   
   gearStatus = new QPushButton("All Gear DOWN");
   gearStatus->setCheckable(true);
   gearStatus->setChecked(true);
   connect(gearStatus, &QPushButton::toggled, this, &MCSWidget::lowerAllGear);

   wheelBreaks = new QPushButton("All Breaks ON");
   wheelBreaks->setCheckable(true);
   wheelBreaks->setChecked(true);
   connect(wheelBreaks, &QPushButton::toggled, this, &MCSWidget::activateAllBreaks);

   layout->addWidget(name);
   layout->addWidget(tail);
   layout->addWidget(pause);
   layout->addWidget(status);
   layout->addWidget(ipSim);
   layout->addWidget(ipCPD);
   layout->addWidget(aircraft);
   layout->addWidget(flaps);
   layout->addWidget(gearStatus);
   layout->addWidget(altitude);
   layout->addWidget(airspeed);
   layout->addWidget(mach);
   layout->addWidget(heading);
   layout->addWidget(fuel);
   layout->addWidget(timeLocal);
   layout->addWidget(timeZulu);
   layout->addWidget(wheelBreaks);

   return layout;
}

void MCSWidget::pauseAllSimulators(bool paused)
{
   qDebug() << "Pausing all simulators.";
   pause->setText(paused ? "All Paused" : "All Running");
   foreach (MCSSlaveWidget* slave, slaveWidgets)
   {
      slave->pauseSimulator(paused);
   }
}

void MCSWidget::lowerAllGear(bool down)
{
   gearStatus->setText(down ? "All Gear DOWN" : "All Gear UP");
   foreach (MCSSlaveWidget* slave, slaveWidgets)
   {
      slave->lowerGear(down);
   }
}

void MCSWidget::activateAllBreaks(bool active)
{
   wheelBreaks->setText(active ? "All Breaks ON" : "All Breaks OFF");
   foreach (MCSSlaveWidget* slave, slaveWidgets)
   {
      slave->activateBreaks(active);
   }
}
