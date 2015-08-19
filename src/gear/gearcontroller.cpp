
#include "gearcontroller.h"

#include <QString>

#include "core/cpdsettings.h"


GearController::GearController(CPDSettings* _settings, QObject* _parent)
: QObject(_parent),
  settings(_settings)
{
   
}

GearController::~GearController()
{
}

void GearController::setGear()
{
   
}
