
#include "singlegearwidget.h"

#include <QString>
#include <QLabel>
#include <QDebug>
#include <QVBoxLayout>

#include "qt-toggle-input/toggle-input.hh"
#include "core/cpdsettings.h"
#include "gearcontroller.h"


SingleGearWidget::SingleGearWidget(CPDSettings* _settings, GearController* _gearC, int _gearNum, QFrame* _parent)
: QFrame(_parent),
  settings(_settings),
  gearC(_gearC),
  gearNum(_gearNum)
{
   QVBoxLayout* layout = new QVBoxLayout(this);
   layout->setContentsMargins(4, 4, 4, 4);

   label = new QLabel((gearNum == -1) ? "Gear" : QString::number(gearNum));
   label->setAlignment(Qt::AlignHCenter);
   label->setObjectName("border-light");
   status = new QLabel("0%");
   status->setAlignment(Qt::AlignHCenter);

   gearSwitch = new ToggleInput(this);
   gearSwitch->setMode(false, true, true);
   connect(gearSwitch, &ToggleInput::positionChanged, this, &SingleGearWidget::switchChanged);

   layout->addWidget(label);
   layout->addWidget(gearSwitch);
   layout->addWidget(status);

   setObjectName("border-light");
}

SingleGearWidget::~SingleGearWidget()
{
}

void SingleGearWidget::setStatus(double percent)
{

}

void SingleGearWidget::setSwitch(bool up)
{
   gearSwitch->setPosition(up);
}
