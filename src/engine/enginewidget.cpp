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

EngineWidget::EngineWidget(HDDSettings* _hddSettings, int _numThrottles, QWidget* _parent)
: QWidget(_parent),
  hddSettings(_hddSettings),
  numThrottles(_numThrottles)
{
   setupEngineControls();
   
   setMinimumSize(QSize(320, 320));
}

//EngineWidget::EngineWidget(const EngineWidget& orig)
//{
//}

EngineWidget::~EngineWidget()
{
}

ThrottleWidget* EngineWidget::createThrottle(int engNum, int max, int value)
{
   ThrottleWidget* widget = new ThrottleWidget();
   
   QLabel* label = new QLabel(QString("ENG %1").arg(engNum));
   label->setMinimumWidth(50);
   widget->label = label;
   
   QProgressBar* throttle = new QProgressBar();
   throttle->setOrientation(Qt::Vertical);
   throttle->setTextDirection(QProgressBar::BottomToTop);
   throttle->setMinimumWidth(10);
   throttle->setMinimum(0);
   throttle->setMaximum(max);
   throttle->setValue(value);
   widget->throttle = throttle;
   
   return widget;
}

void EngineWidget::setupEngineControls()
{
   QGridLayout* layout = new QGridLayout(this);
   layout->setContentsMargins(0, 0, 0, 0);
   
   for (int i = 0; i < numThrottles; i++) {
      ThrottleWidget* throttle = createThrottle(i, 100, 0);
      throttles.append(throttle->throttle);
      
      layout->addWidget(throttle->label, 0, i, 1, 1);
      layout->addWidget(throttle->throttle, 1, i, 5, 1);
   }
}

void EngineWidget::updateThrottleCommand(float throttle, int engNum)
{
   // If this throttle is unknown, or if the throttle is 0.75 (the default if it does not exist)
   if (engNum > numThrottles || throttle == 0.75) {
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
