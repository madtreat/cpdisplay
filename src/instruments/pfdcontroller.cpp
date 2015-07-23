/*
 * File:   pfdcontroller.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 11, 2015, 10:58 AM
 */

#include "pfdcontroller.h"
#include "core/mapconsts.h"

PFDController::PFDController(QObject* _parent)
: QObject(_parent)
{
   pfdWidget = new WidgetPFD();
   pfdWidget->setContentsMargins(0, 0, 0, 0);
   pfdWidget->setMinimumSize(QSize(DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT));
   pfdWidget->resize(DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT);
   pfdWidget->setObjectName("border");
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

