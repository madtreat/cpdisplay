
#include "singlegearwidget.h"

#include <QString>

#include "core/cpdsettings.h"
#include "gearcontroller.h"


SingleGearWidget::SingleGearWidget(CPDSettings* _settings, GearController* _gearC, QFrame* _parent)
: QFrame(_parent),
  settings(_settings),
  gearC(_gearC)
{
}

SingleGearWidget::~SingleGearWidget()
{
}
