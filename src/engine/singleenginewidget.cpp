/* 
 * File:   singleenginewidget.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on July 31, 2015, 12:54 PM
 */

#include "singleenginewidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "core/hddsettings.h"

#include "engineconsts.h"
#include "enginecontroller.h"
#include "enginedial.h"
#include "throttlewidget.h"


SingleEngineWidget::SingleEngineWidget(HDDSettings* _hddSettings, EngineController* _engC, int _engNum, QWidget* _parent)
: QWidget(_parent),
  hddSettings(_hddSettings),
  engC(_engC),
  engNum(_engNum)
{
   QVBoxLayout* layout = new QVBoxLayout(this);
   layout->setContentsMargins(0, 0, 0, 0);
   
   throttle = new ThrottleWidget(engC, engNum, 100, 0);
   oilT = new EngineDial(engC, engNum, DIAL_OIL_TEMP, 20, 180, 0);
   oilP = new EngineDial(engC, engNum, DIAL_OIL_PRESSURE, 40, 120, 0);
   engRPM = new EngineDial(engC, engNum, DIAL_ENG_RPM, 0, 5000, 0);
   
   layout->addWidget(throttle);
   layout->addWidget(oilT);
   layout->addWidget(oilP);
   layout->addWidget(engRPM);
}

//SingleEngineWidget::SingleEngineWidget(const SingleEngineWidget& orig)
//{
//}

SingleEngineWidget::~SingleEngineWidget()
{
}

void SingleEngineWidget::setOilPressure(float pressure, int _engNum)
{
   if (_engNum == engNum) {
      oilP->setValue(pressure, engNum);
   }
}

void SingleEngineWidget::setOilTemp(float temp, int _engNum)
{
   if (_engNum == engNum) {
      oilT->setValue(temp, engNum);
   }
}

void SingleEngineWidget::setEngRPM(float rpm, int _engNum)
{
   if (_engNum == engNum) {
      engRPM->setValue(rpm, engNum);
   }
}
