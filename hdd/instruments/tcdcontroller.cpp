/* 
 * File:   tcdcontroller.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 11, 2015, 12:02 PM
 */

#include "tcdcontroller.h"

TCDController::TCDController(QObject* _parent)
: QObject(_parent)
{
   tcdWidget = new WidgetTC();
}

TCDController::TCDController(const TCDController& orig)
{
   if (orig.tcdWidget) {
      tcdWidget = orig.tcdWidget;
   }
   else {
      tcdWidget = new WidgetTC();
   }
}

TCDController::~TCDController()
{
   delete tcdWidget;
}

