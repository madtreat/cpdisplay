
#include "gearcontroller.h"

#include <QString>

#include "core/cpdsettings.h"
#include "core/xplanedref.h"


GearController::GearController(CPDSettings* _settings, int _numGears, QObject* _parent)
: QObject(_parent),
  settings(_settings),
  numGears(_numGears)
{
   
}

GearController::~GearController()
{
}

// Note the default for my gear widgets is up, while the dataref is down
void GearController::notifyXPlaneGearHandle(bool up)
{
   QString drefStr(XPDR_GEAR_HANDLE_STATUS);
   emit updateXPlaneGearHandle(drefStr, (int) !up);
}

void GearController::notifyXPlaneFlapHandle(float value)
{
   QString drefStr(XPDR_FLAP_HANDLE_DEPLOY);
   emit updateXPlaneFlaps(drefStr, (float) value/100);
}

