/* 
 * File:   commscontroller.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 23, 2015, 12:33 PM
 */

#include "commscontroller.h"

#include <QDebug>

#include "core/hddsettings.h"
#include "commswidget.h"

CommsController::CommsController(HDDSettings* _hddSettings, QObject* _parent)
: QObject(_parent),
  hddSettings(_hddSettings)
{
   commsWidget = new CommsWidget(hddSettings);
}

//CommsController::CommsController(const CommsController& orig)
//{
//}

CommsController::~CommsController()
{
}
