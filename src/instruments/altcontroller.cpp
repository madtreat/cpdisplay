/* 
 * File:   altcontroller.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 11, 2015, 11:35 AM
 */

#include "altcontroller.h"

ALTController::ALTController(QObject* _parent)
: QObject(_parent)
{
   altWidget = new WidgetALT();
}

ALTController::ALTController(const ALTController& orig)
{
   if (orig.altWidget) {
      altWidget = orig.altWidget;
   }
   else {
      altWidget = new WidgetALT();
   }
}

ALTController::~ALTController()
{
   delete altWidget;
}

