//
//  singlefuelwidget.cpp
//  
//
//  Created by Madison Treat on 8/10/15.
//
//

#include "singlefuelwidget.h"

#include <QVBoxLayout>
#include <QString>
#include <QLabel>

#include "core/cpdsettings.h"
#include "enginecontroller.h"
#include "enginedial.h"


SingleFuelWidget::SingleFuelWidget(CPDSettings* _cpdSettings, EngineController* _engC, int _engNum, QFrame* _parent)
: cpdSettings(_cpdSettings),
  engC(_engC),
  engNum(_engNum)
{
   QVBoxLayout* layout = new QVBoxLayout(this);
   layout->setContentsMargins(0, 0, 0, 0);

   QString labelText = QString("ENG %1").arg(engNum);
   QLabel* label = new QLabel(labelText);
   label->setAlignment(Qt::AlignHCenter);
   label->setObjectName("border-light");

   // Making engines with 0-100% fuel capacity.  Could make this an optional
   // volume later, instead of percent.
   gauge = new EngineDial(engC, engNum, DIAL_FUEL, 0, 100, 100);

   layout->addWidget(label);
   layout->addWidget(gauge);
   layout->addStretch(2);

   setObjectName("border-light");
}

SingleFuelWidget::~SingleFuelWidget()
{

}

void SingleFuelWidget::setValuePercent(float value, int _engNum)
{
   if (_engNum == engNum) {
      valuePercent = value;
   }
}


void SingleFuelWidget::setValueVolume(float value, int _engNum)
{
   if (_engNum == engNum) {
      valueVolume = value;
   }
}
