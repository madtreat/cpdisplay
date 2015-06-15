/* 
 * File:   asicontroller.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 11, 2015, 11:40 AM
 */

#include "asicontroller.h"

ASIController::ASIController(QObject* _parent)
: QObject(_parent)
{
   asiWidget = new WidgetASI();
}

ASIController::ASIController(const ASIController& orig)
{
   if (orig.asiWidget) {
      asiWidget = orig.asiWidget;
   }
   else {
      asiWidget = new WidgetASI();
   }
}

ASIController::~ASIController()
{
   delete asiWidget;
}

