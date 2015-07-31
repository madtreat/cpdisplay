/* 
 * File:   enginecontroller.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 23, 2015, 12:33 PM
 */

#include "enginecontroller.h"

#include <QDebug>

#include "core/hddsettings.h"

EngineController::EngineController(HDDSettings* _hddSettings, int _numEngines, QObject* _parent)
: QObject(_parent),
  hddSettings(_hddSettings),
  numEngines(_numEngines)
{
//   engineWidget = new EngineWidget(hddSettings, _numThrottles);
}

//EngineController::EngineController(const EngineController& orig)
//{
//}

EngineController::~EngineController()
{
}

void EngineController::updateNumEngines(int numEng)
{
   numEngines = numEng;
   //setupEngineControls();
}

void EngineController::updateThrottleCommand(float throttle, int engNum)
{
   // If this throttle is unknown, or if the throttle is 0.75 (the default if it does not exist)
   if (engNum > numEngines-1 || throttle == 0.75) {
//      QProgressBar* throttle = createThrottle(100, (int) throttle);
//      throttles.append(throttle);
      return;
   }
//   qDebug() << "Updating engine" << engNum << "to" << 100*throttle;
//   throttles.at(engNum)->setValue((int) 100*throttle);
   emit throttleUpdate((int) 100*throttle, engNum);
}

void EngineController::updateThrottleActual(float throttle, int engNum)
{
   
}

void EngineController::updateEngPower(float power, int engNum)
{
   
}

void EngineController::updateEngThrust(float thrust, int engNum)
{
   
}

void EngineController::updateEngTorque(float torque, int engNum)
{
   
}

void EngineController::updateEngRPM(float rpm, int engNum)
{
   
}

void EngineController::updateOilTemp(float temp, int engNum)
{
   if (engNum == 0) {
      emit oilTUpdate(temp);
   }
}

void EngineController::updateOilPressure(float pressure, int engNum)
{
   if (engNum == 0) {
      emit oilPUpdate(pressure);
   }
}
