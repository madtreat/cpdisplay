/* 
 * File:   hsicontroller.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 11, 2015, 11:46 AM
 */

#include "hsicontroller.h"

HSIController::HSIController(QObject* _parent)
: QObject(_parent)
{
   hsiWidget = new WidgetHSI();
}

HSIController::HSIController(const HSIController& orig)
{
   if (orig.hsiWidget) {
      hsiWidget = orig.hsiWidget;
   }
   else {
      hsiWidget = new WidgetHSI();
   }
}

HSIController::~HSIController()
{
   delete hsiWidget;
}

