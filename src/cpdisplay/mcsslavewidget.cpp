#include "mcsslavewidget.h"

#include <QVBoxLayout>

#include "core/cpdsettings.h"
#include "core/switchboard.h"


MCSSlaveWidget::MCSSlaveWidget(CPDSettings* _settings, SwitchBoard* _sb, int _slaveID, QFrame* _parent)
: QFrame(_parent),
  settings(_settings),
  sb(_sb),
  slaveID(_slaveID)
{
   setObjectName("include-background");
   slaveConfig = settings->getSlave(slaveID);
   QVBoxLayout* layout = new QVBoxLayout(this);

   name = new QLabel(slaveConfig->m_slaveName);
   layout->addWidget(name);

   pause = new QPushButton("Resume");
   pause->setCheckable(true);
   pause->setChecked(true);
   pause->setEnabled(slaveConfig->m_allowMCSOverride);
   connect(pause, &QPushButton::toggled, this, &MCSSW::pauseSimulator);
   layout->addWidget(pause);

   status = new QLabel("--STATUS--");
   layout->addWidget(status);

   ipSim = new QLabel(slaveConfig->m_xplaneHost.toString());
   layout->addWidget(ipSim);

   ipCPD = new QLabel(slaveConfig->m_cpdHost.toString());
   layout->addWidget(ipCPD);

   aircraft = new QLabel("--AIRCRAFT--");
   layout->addWidget(aircraft);

   flaps = new QLabel("--FLAPS--");
   layout->addWidget(flaps);

   gearStatus = new QPushButton("Down");
   gearStatus->setCheckable(true);
   gearStatus->setChecked(true);
   gearStatus->setEnabled(slaveConfig->m_allowMCSOverride);
   connect(gearStatus, &QPushButton::toggled, this, &MCSSW::lowerGear);
   layout->addWidget(gearStatus);

   altitude = new QLabel("--ALTITUDE--");
   layout->addWidget(altitude);

   airspeed = new QLabel("--AIRSPEED--");
   layout->addWidget(airspeed);

   mach = new QLabel("--MACH--");
   layout->addWidget(mach);

   heading = new QLabel("--HEADING--");
   layout->addWidget(heading);

   fuel = new QLabel("--FUEL--");
   layout->addWidget(fuel);

   timeLocal = new QLabel("--LOCAL TIME--");
   layout->addWidget(timeLocal);

   timeZulu = new QLabel("--ZULU TIME--");
   layout->addWidget(timeZulu);

   wheelBreaks = new QPushButton("Breaks on");
   wheelBreaks->setCheckable(true);
   wheelBreaks->setChecked(true);
   wheelBreaks->setEnabled(slaveConfig->m_allowMCSOverride);
   connect(wheelBreaks, &QPushButton::toggled, this, &MCSSW::activateBreaks);
   layout->addWidget(wheelBreaks);
}

MCSSlaveWidget::~MCSSlaveWidget()
{
}

void MCSSlaveWidget::pauseSimulator(bool paused)
{
   pause->setText(paused ? "Resume" : "Pause");
   // TODO: send signal to sim
}

void MCSSlaveWidget::lowerGear(bool down)
{
   gearStatus->setText(down ? "Raise" : "Lower");
   // TODO: send signal to sim
}

void MCSSlaveWidget::activateBreaks(bool active)
{
   wheelBreaks->setText(active ? "Off" : "Break");
   // TODO: send signal to sim
}
