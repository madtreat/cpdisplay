/* 
 * File:   hddcontroller.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 15, 2015, 6:39 PM
 */

#include "hddcontroller.h"
#include "hddsettings.h"
#include "switchboard.h"
#include "window.h"

#include "mapcontroller.h"
#include "pfdcontroller.h"
#include "altcontroller.h"
#include "asicontroller.h"
#include "hsicontroller.h"

HDDController::HDDController(HDDSettings* _settings, QObject* _parent)
: QObject(_parent)
{
   settings = _settings;
   sb = new SwitchBoard(settings);
   window = new HDDWindow(settings);
   
   connectSignals();
   
   window->show();
}

//HDDController::HDDController(const HDDController& orig)
//{
//}

HDDController::~HDDController()
{
}


/*
 * Connects the switchboard directly to the widgets.
 */
void HDDController::connectSignals()
{
   MapController* mapC = window->getMapC();
   PFDController* pfdC = window->getPFDC();
   ALTController* altC = window->getALTC();
   ASIController* asiC = window->getASIC();
   HSIController* hsiC = window->getHSIC();

   MapView*    mapView = window->getMapView();
   MapOverlay* overlay = window->getOverlay();

   // Speeds
   connect(sb, SIGNAL(speedUpdate(float)), pfdC,    SLOT(setAirspeed(float)));
   connect(sb, SIGNAL(speedUpdate(float)), asiC,    SLOT(setAirspeed(float)));

   // Compass/Heading
   connect(sb, SIGNAL(compassUpdate(float)), mapView, SLOT(setHeading(float)));
   connect(sb, SIGNAL(compassUpdate(float)), overlay, SLOT(setHeading(float)));
   connect(sb, SIGNAL(compassUpdate(float)), pfdC,    SLOT(setHeading(float)));
   connect(sb, SIGNAL(compassUpdate(float)), hsiC,    SLOT(setHeading(float)));

   // Positions (this AC and others)
   connect(sb, SIGNAL(latLonUpdate(float, float, int)), window, SLOT(latLonUpdate(float, float, int)));

   // Altitudes
   connect(sb, SIGNAL(altMSLUpdate(float)), pfdC,     SLOT(setAltitude(float)));
   connect(sb, SIGNAL(altMSLUpdate(float)), altC,     SLOT(setAltitude(float)));

   //connect(sb, SIGNAL(altAGLUpdate(float)), pfdC,     SLOT(setAltitude(float)));
   //connect(sb, SIGNAL(altAGLUpdate(float)), altC,     SLOT(setAltitude(float)));
}
