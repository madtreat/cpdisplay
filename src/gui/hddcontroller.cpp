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
 * Connects the switchboard to the window.
 */
void HDDController::connectSignals()
{
   // connect sb signals to window slots
   connect(sb, SIGNAL(compassUpdate(float)), window, SLOT(compassUpdate(float)));
   connect(sb, SIGNAL(latLonUpdate(float, float, int)), window, SLOT(latLonUpdate(float, float, int)));
   connect(sb, SIGNAL(altMSLUpdate(float)), window, SLOT(altMSLUpdate(float)));
   connect(sb, SIGNAL(altAGLUpdate(float)), window, SLOT(altAGLUpdate(float)));
}
