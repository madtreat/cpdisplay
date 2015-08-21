
#include "gearwidget.h"

#include <QString>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>

#include "core/cpdsettings.h"
#include "gearcontroller.h"
#include "singlegearwidget.h"


GearWidget::GearWidget(CPDSettings* _settings, GearController* _gearC, int _numGears, QFrame* _parent)
: QFrame(_parent),
  settings(_settings),
  gearC(_gearC),
  numGears(_numGears)
{
   QHBoxLayout* layout = new QHBoxLayout(this);

   master = new SingleGearWidget(settings, gearC, -1, this);
   layout->addWidget(master);

   for (int i = 0; i < MAX_NUM_GEARS; i++) {
      SingleGearWidget* gear = new SingleGearWidget(settings, gearC, i, this);
      connect(master, &SingleGearWidget::switchChanged, gear, &SingleGearWidget::setSwitch);
      connect(master, &SingleGearWidget::switchChanged, this, &GearWidget::masterChanged);

      if (i >= numGears) {
         gear->setVisible(false);
      }
      gears.append(gear);
      layout->addWidget(gear);
   }

   setObjectName("border");
}

GearWidget::~GearWidget()
{
}
