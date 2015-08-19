
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

   label = new QLabel((gearNum == -1) ? "Master" : QString::number(gearNum));
   label->setAlignment(Qt::AlignHCenter);
   status = new QLabel("0%");
   status->setAlignment(Qt::AlignHCenter);

   gearSwitch = new ToggleInput(this);
   gearSwitch->setMode(false, true, true);
   connect(gearSwitch, &ToggleInput::positionChanged, this, &SingleGearWidget::switchChanged);

   layout->addWidget(label);
   layout->addWidget(gearSwitch);
   layout->addWidget(status);

   setObjectName("border");
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
