/*
 * File:   pfdcontroller.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 11, 2015, 10:58 AM
 */

#include "pfdcontroller.h"

PFDController::PFDController(QObject* _parent)
: QObject(_parent)
{
   pfdWidget = new WidgetPFD();
   pfdWidget->setContentsMargins(0, 0, 0, 0);
}

PFDController::PFDController(const PFDController& orig)
{
   if (orig.pfdWidget) {
      pfdWidget = orig.pfdWidget;
   }
   else {
      pfdWidget = new WidgetPFD();
   }
}

PFDController::~PFDController()
{
   delete pfdWidget;
}

