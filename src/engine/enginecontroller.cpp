/* 
 * File:   enginecontroller.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 23, 2015, 12:33 PM
 */

#include "enginecontroller.h"

#include <QDebug>

#include "core/hddsettings.h"
#include "enginewidget.h"

EngineController::EngineController(HDDSettings* _hddSettings, int _numThrottles, QObject* _parent)
: QObject(_parent),
  hddSettings(_hddSettings)
{
   engineWidget = new EngineWidget(hddSettings, _numThrottles);
}

//EngineController::EngineController(const EngineController& orig)
//{
//}

EngineController::~EngineController()
{
}
