
#include "gearwidget.h"

#include <QString>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

#include "core/cpdsettings.h"
#include "core/xplanedref.h"
#include "gearcontroller.h"
#include "singlegearwidget.h"


GearWidget::GearWidget(CPDSettings* _settings, GearController* _gearC, int _numGears, QFrame* _parent)
: QFrame(_parent),
  settings(_settings),
  gearC(_gearC),
  numGears(_numGears)
{
   connect(gearC, &GEARC::updateGearRetractable, this, &GearWidget::setVisible);
   QVBoxLayout* layout = new QVBoxLayout(this);

   master = new SingleGearWidget(settings, gearC, -1, this);
   connect(master, &SingleGearWidget::switchChanged, gearC, &GEARC::notifyXPlaneGearHandle);
   layout->addWidget(master);

   setObjectName("border");
}

GearWidget::~GearWidget()
{
}
