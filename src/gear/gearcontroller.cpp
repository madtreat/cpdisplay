
#include "gearcontroller.h"

#include <QString>

#include "core/cpdsettings.h"
#include "core/switchboard.h"
#include "core/xplanedref.h"


GearController::GearController(
  CPDSettings* _settings,
  SwitchBoard* _sb,
  int _numGears,
  QObject* _parent
)
: QObject(_parent),
settings(_settings),
sb(_sb),
numGears(_numGears) {
  connect(sb, &SWB::gearRetractableUpdate, this, &GEARC::updateGearRetractable);
  connect(sb, &SWB::flapUpdate,            this, &GEARC::flapUpdate);
  connect(sb, &SWB::flapHandleUpdate,      this, &GEARC::flapHandleUpdate);

  connect(this, &GEARC::updateXPlaneGearHandle, sb, &SWB::sendDREF);
  connect(this, &GEARC::updateXPlaneFlaps,      sb, &SWB::sendDREF);
}

GearController::~GearController() {
}

// Note the default for my gear widgets is up, while the dataref is down
void GearController::notifyXPlaneGearHandle(bool up) {
  QString drefStr(XPDR_GEAR_HANDLE_STATUS);
  emit updateXPlaneGearHandle(drefStr, (int) !up);
}

void GearController::notifyXPlaneFlapHandle(float value) {
  QString drefStr(XPDR_FLAP_HANDLE_DEPLOY);
  emit updateXPlaneFlaps(drefStr, (float) value/100);
}

