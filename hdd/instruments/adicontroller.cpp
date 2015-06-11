/* 
 * File:   adicontroller.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 11, 2015, 10:58 AM
 */

#include "adicontroller.h"

ADIController::ADIController(QObject* _parent)
: QObject(_parent)
{
   adiWidget = new WidgetADI();
}

ADIController::ADIController(const ADIController& orig)
{
   if (orig.adiWidget) {
      adiWidget = orig.adiWidget;
   }
   else {
      adiWidget = new WidgetADI();
   }
}

ADIController::~ADIController()
{
   delete adiWidget;
}

