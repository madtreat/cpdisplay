
#include "singlegearwidget.h"

#include <QString>
#include <QLabel>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>

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
   QHBoxLayout* center = new QHBoxLayout();
//   layout->setContentsMargins(4, 4, 4, 4);
   layout->setContentsMargins(0, 0, 0, 0);
   center->setContentsMargins(0, 0, 0, 0);

   label = new QLabel((gearNum == -1) ? "Gear" : QString::number(gearNum));
   label->setAlignment(Qt::AlignHCenter);
   label->setObjectName("border-light");
   status = new QLabel("0%");
   status->setAlignment(Qt::AlignHCenter);

   gearSwitch = new ToggleInput(this);
   gearSwitch->setMode(false, true, true);
   connect(gearSwitch, &ToggleInput::positionChanged, this, &SingleGearWidget::switchChanged);

   center->addStretch();
   center->addWidget(gearSwitch);
   center->addStretch();

   layout->addWidget(label);
   layout->addLayout(center);
   layout->addWidget(status);

//   setObjectName("border-light");
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
