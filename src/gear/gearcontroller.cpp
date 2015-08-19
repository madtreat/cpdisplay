
#include "gearcontroller.h"

#include <QString>

#include "core/cpdsettings.h"


GearController::GearController(CPDSettings* _settings, int _numGears, QObject* _parent)
: QObject(_parent),
  settings(_settings),
  numGears(_numGears)
{
   
}

GearController::~GearController()
{
}

void GearController::setGear()
{
   
}
