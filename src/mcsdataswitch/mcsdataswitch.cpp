
#include "mcsdataswitch.h"

#include "core/cpdsettings.h"
#include "core/switchboard.h"
#include "core/xplanedata.h"
#include "core/xplanedref.h"


MCSDataSwitch::MCSDataSwitch(CPDSettings* _settings, QObject* _parent)
: QObject(_parent),
mcsSettings(_settings) {
  SlaveMap slaveMap = mcsSettings->slaves();
  foreach (SlaveSystem* slave, slaveMap.values()) {
    int id = slave->m_slaveID;
    if (id != 2 && id != 3) {
      continue;
    }
    SwitchBoard* sb = new SwitchBoard(mcsSettings, id, true, this);
    slaves[id] = sb;
  }
}

MCSDataSwitch::~MCSDataSwitch() {
  // NOTE: Qt manages the slaves, and is thus in charge of emancipation.
}

