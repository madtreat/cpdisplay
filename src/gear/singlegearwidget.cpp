
#include "singlegearwidget.h"

#include <QString>

#include "core/cpdsettings.h"
#include "gearcontroller.h"


SingleGearWidget::SingleGearWidget(CPDSettings* _settings, GearController* _gearC, int _gearNum, QFrame* _parent)
: QFrame(_parent),
  settings(_settings),
  gearC(_gearC),
  gearNum(_gearNum)
{
   setObjectName("border");
}

SingleGearWidget::~SingleGearWidget()
{
}

void SingleGearWidget::setStatus(double percent)
{

}
