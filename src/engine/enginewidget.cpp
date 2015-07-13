/* 
 * File:   enginewidget.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 15, 2015, 2:36 PM
 */

#include "enginewidget.h"

#include <QGridLayout>
#include <QLabel>

#include "core/hddsettings.h"

EngineWidget::EngineWidget(HDDSettings* _hddSettings, int _numEngines, QWidget* _parent)
: QWidget(_parent),
  hddSettings(_hddSettings),
  numEngines(_numEngines)
{
   setupEngineControls();
   
   setMinimumSize(QSize(65, 320));
}

//EngineWidget::EngineWidget(const EngineWidget& orig)
//{
//}

EngineWidget::~EngineWidget()
{
}

void EngineWidget::setupEngineControls()
{
   QGridLayout* layout = new QGridLayout(this);
   layout->setContentsMargins(0, 0, 0, 0);
   
   // This loop will create throttles in pairs, to keep them grouped better on the main layout
   for (int i = 0; i < numEngines/2; i++) {
      QHBoxLayout* pairLayout = new QHBoxLayout();
      ThrottleWidget* lt = new ThrottleWidget(i*2, 100, 0); // left throttle of pair
      pairLayout->addWidget(lt);
      throttles.append(lt);
      
      // If numEngines is even, add a second throttle to this pair
      if (numEngines % 2 == 0) {
         ThrottleWidget* rt = new ThrottleWidget((i*2)+1, 100, 0); // right throttle of pair
         pairLayout->addWidget(rt);
         throttles.append(rt);
      }
      
      layout->addLayout(pairLayout, 0, i, 3, 1);
   }
}

void EngineWidget::updateThrottleCommand(float throttle, int engNum)
{
   // If this throttle is unknown, or if the throttle is 0.75 (the default if it does not exist)
   if (engNum > numEngines-1 || throttle == 0.75) {
//      QProgressBar* throttle = createThrottle(100, (int) throttle);
//      throttles.append(throttle);
      return;
   }
//   qDebug() << "Updating engine" << engNum << "to" << 100*throttle;
   throttles.at(engNum)->setValue((int) 100*throttle);
}

void EngineWidget::updateThrottleActual(float throttle, int engNum)
{
   
}

void EngineWidget::updateEngPower(float power, int engNum)
{
   
}

void EngineWidget::updateEngThrust(float thrust, int engNum)
{
   
}

void EngineWidget::updateEngTorque(float torque, int engNum)
{
   
}

void EngineWidget::updateEngRPM(float rpm, int engNum)
{
   
}
