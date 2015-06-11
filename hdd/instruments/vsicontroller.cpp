/* 
 * File:   vsicontroller.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 11, 2015, 12:02 PM
 */

#include "vsicontroller.h"

VSIController::VSIController(QObject* _parent)
: QObject(_parent)
{
   vsiWidget = new WidgetVSI();
}

VSIController::VSIController(const VSIController& orig)
{
   if (orig.vsiWidget) {
      vsiWidget = orig.vsiWidget;
   }
   else {
      vsiWidget = new WidgetVSI();
   }
}

VSIController::~VSIController()
{
   delete vsiWidget;
}

