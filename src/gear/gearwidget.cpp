
#include "gearwidget.h"

#include <QString>
#include <QPushButton>
#include <QLabel>

#include "core/cpdsettings.h"
#include "gearcontroller.h"
#include "singlegearwidget.h"


GearWidget::GearWidget(CPDSettings* _settings, GearController* _gearC, QFrame* _parent)
: QFrame(_parent),
  settings(_settings),
  gearC(_gearC)
{
   for (int i = 0; i < MAX_NUM_GEARS; i++) {
      SingleGearWidget* gear = new SingleGearWidget(settings, gearC, this);
      gears.append(gear);
   }

   setObjectName("border");
}

GearWidget::~GearWidget()
{
}
