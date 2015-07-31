/* 
 * File:   oilwidget.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on July 31, 2015, 11:01 AM
 */

#include "enginedialwidget.h"

#include <QLabel>
#include <QVBoxLayout>

#include "enginecontroller.h"
#include "enginedial.h"


EngineDialWidget::EngineDialWidget(EngineController* _engC, EngineDialType _type, int _min, int _max, int _value, QWidget* _parent)
: QWidget(_parent),
  engC(_engC),
  type(_type),
  valueMax(_max),
  valueMin(_min),
  value(_value)
{
   QString labelText;
   int stepSize = 5;
   if (type == OIL_TEMP) {
      connect(engC, &EngineController::oilTUpdate, this, &EngineDialWidget::setValue);
      labelText = "Oil T (_X_ deg C)";
      stepSize = 10;
   }
   else if (type == OIL_PRESSURE) {
      connect(engC, &EngineController::oilPUpdate, this, &EngineDialWidget::setValue);
      labelText = "Oil P (_X_ psi)";
      stepSize = 10;
   }
   labelBase = labelText;
   
   label = new QLabel(labelText.replace("_X_", QString::number(value)));
   label->setAlignment(Qt::AlignHCenter);
   
//   dial = new QDial(this);
   dial = new EngineDial(type, this);
   dial->setRange(valueMin, valueMax);
   dial->setValue(value);
   dial->setWrapping(false);
   dial->setNotchesVisible(true);
   dial->setSingleStep(stepSize);
   dial->setEnabled(false);
   
   QVBoxLayout* layout = new QVBoxLayout(this);
   layout->setContentsMargins(0, 0, 0, 0);
   layout->addWidget(dial);
//   layout->addWidget(label);
   
   setMaximumSize(QSize(120, 120));
}

//OilWidget::OilWidget(const OilWidget& orig)
//{
//}

EngineDialWidget::~EngineDialWidget()
{
}

void EngineDialWidget::setValue(float _value)
{
   value = _value;
   dial->setValue(value);
   QString text = labelBase.replace("_X_", QString::number(value));
   label->setText(text);
}
