#include "mcsslavewidget.h"

#include <QVBoxLayout>

#include "core/cpdsettings.h"
#include "core/switchboard.h"


MCSSlaveWidget::MCSSlaveWidget(
  CPDSettings* _settings,
  SwitchBoard* _sb,
  int _slaveID,
  QFrame* _parent
)
: QFrame(_parent),
settings(_settings),
sb(_sb),
slaveID(_slaveID) {
  setObjectName("include-background");
  slaveConfig = settings->getSlave(slaveID);
  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  name        = new QLabel(slaveConfig->m_slaveName);
  tail        = new QLabel("--TAIL--");
  status      = new QLabel("--STATUS--");
  ipSim       = new QLabel(slaveConfig->m_xplaneHost.toString());
  ipCPD       = new QLabel(slaveConfig->m_cpdHost.toString());
  aircraft    = new QLabel("--AIRCRAFT--");
  flaps       = new QLabel("--FLAPS--");
  altitude    = new QLabel("--ALTITUDE--");
  airspeed    = new QLabel("--AIRSPEED--");
  mach        = new QLabel("--MACH--");
  heading     = new QLabel("--HEADING--");
  fuel        = new QLabel("--FUEL--");
  timeLocal   = new QLabel("--LOCAL TIME--");
  timeZulu    = new QLabel("--ZULU TIME--");

  pause = new QPushButton("Resume");
  pause->setCheckable(true);
  pause->setChecked(true);
  pause->setEnabled(slaveConfig->m_allowMCSOverride);
  connect(pause, &QPushButton::toggled, this, &MCSSW::pauseSimulator);

  gearStatus = new QPushButton("Down");
  gearStatus->setCheckable(true);
  gearStatus->setChecked(true);
  gearStatus->setEnabled(slaveConfig->m_allowMCSOverride);
  connect(gearStatus, &QPushButton::toggled, this, &MCSSW::lowerGear);

  wheelBreaks = new QPushButton("Breaks on");
  wheelBreaks->setCheckable(true);
  wheelBreaks->setChecked(true);
  wheelBreaks->setEnabled(slaveConfig->m_allowMCSOverride);
  connect(wheelBreaks, &QPushButton::toggled, this, &MCSSW::activateBreaks);

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
}

MCSSlaveWidget::~MCSSlaveWidget() {
}

void MCSSlaveWidget::pauseSimulator(bool paused) {
  pause->setText(paused ? "Paused" : "Running");
  pause->setChecked(paused); // when overriden by MCS, must change it
  sb->pauseSimulator(paused);
}

void MCSSlaveWidget::lowerGear(bool down) {
  gearStatus->setText(down ? "Gear is DOWN" : "Gear is UP");
  gearStatus->setChecked(down);
  sb->sendGearHandle(down);
}

void MCSSlaveWidget::activateBreaks(bool active) {
  wheelBreaks->setText(active ? "Breaks are ON" : "Breaks are OFF");
  wheelBreaks->setChecked(active);
  sb->sendBreaksOn(active);
}
