/* 
 * File:   hddcontroller.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 15, 2015, 6:39 PM
 */

#include "hddcontroller.h"

#include <math.h>

#include "hddsettings.h"
#include "switchboard.h"
#include "gui/window.h"

#include "map/mapcontroller.h"
#include "instruments/adicontroller.h"
#include "instruments/altcontroller.h"
#include "instruments/asicontroller.h"
#include "instruments/hsicontroller.h"
#include "instruments/pfdcontroller.h"
#include "instruments/tcdcontroller.h"
#include "instruments/vsicontroller.h"

#include "comms/commscontroller.h"
#include "engine/enginecontroller.h"
#include "traffic/trafficcontroller.h"

HDDController::HDDController(HDDSettings* _settings, QObject* _parent)
: QObject(_parent)
{
   settings = _settings;
   acMap = new QMap<int, Aircraft*>();
   
   sb = new SwitchBoard(settings);
   window = new HDDWindow(settings, acMap, this);

   angVelUpdatedFlag = false;
   pitchUpdatedFlag = false;
   rollUpdatedFlag = false;

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
   ADIController* adiC = window->getADIC();
   ALTController* altC = window->getALTC();
   ASIController* asiC = window->getASIC();
   HSIController* hsiC = window->getHSIC();
   PFDController* pfdC = window->getPFDC();
   TCDController* tcdC = window->getTCDC();
   VSIController* vsiC = window->getVSIC();

   CommsController* comC =   window->getComC();
   EngineController* engC =  window->getEngC();
   TrafficController* tfcC = window->getTfcC();

   MapView*    mapView = window->getMapView();
   MapOverlay* overlay = window->getOverlay();
   
   CommsWidget* comW = comC->getWidget();
   EngineWidget* engW = engC->getWidget();
//   TrafficWidget* tfcW = tfcC->getWidget();

   // Self-calculated turn rate
   connect(this, SIGNAL(turnRateUpdate(float)), pfdC, SLOT(setTurnRate(float)));
   connect(this, SIGNAL(turnRateUpdate(float)), tcdC, SLOT(setTurnRate(float)));
   
   // Times
   connect(sb, SIGNAL(timeUpdate(float, float)), comW, SLOT(setTimes(float, float)));

   // Speeds
   connect(sb, SIGNAL(speedUpdate(float)),   asiC,    SLOT(setAirspeed(float)));
   connect(sb, SIGNAL(speedUpdate(float)),   pfdC,    SLOT(setAirspeed(float)));

   // Mach, Vertical Velocity
   connect(sb, SIGNAL(machNumUpdate(float)), pfdC,    SLOT(setMachNo(float)));

   connect(sb, SIGNAL(vertVelUpdate(float)), pfdC,    SLOT(setClimbRate(float)));
   connect(sb, SIGNAL(vertVelUpdate(float)), vsiC,    SLOT(setClimbRate(float)));

   // Pressure
   connect(sb, SIGNAL(pressureUpdate(float)), altC,   SLOT(setPressure(float)));
   connect(sb, SIGNAL(pressureUpdate(float)), pfdC,   SLOT(setPressure(float)));

   // Angular Velocities (Q, P, R)
   connect(sb, SIGNAL(angVelUpdate(float, float, float)), this, SLOT(updateAngVel(float, float, float)));

   // Pitch, Roll, Heading
   connect(sb, SIGNAL(pitchUpdate(float)),   this,    SLOT(updatePitch(float)));
   connect(sb, SIGNAL(pitchUpdate(float)),   adiC,    SLOT(setPitch(float)));
   connect(sb, SIGNAL(pitchUpdate(float)),   pfdC,    SLOT(setPitch(float)));

   connect(sb, SIGNAL(rollUpdate(float)),    this,    SLOT(updateRoll(float)));
   connect(sb, SIGNAL(rollUpdate(float)),    adiC,    SLOT(setRoll(float)));
   connect(sb, SIGNAL(rollUpdate(float)),    pfdC,    SLOT(setRoll(float)));

   connect(sb, SIGNAL(headingMagUpdate(float)), mapView, SLOT(setHeading(float)));
   connect(sb, SIGNAL(headingMagUpdate(float)), overlay, SLOT(setHeading(float)));
   connect(sb, SIGNAL(headingMagUpdate(float)), hsiC,    SLOT(setHeading(float)));
   connect(sb, SIGNAL(headingMagUpdate(float)), pfdC,    SLOT(setHeading(float)));

   // AOA, SideSlip
   connect(sb, SIGNAL(aoaSideSlipUpdate(float, float)), pfdC, SLOT(setFlightPathMarker(float, float)));

   // these are not the values
   //connect(sb, SIGNAL(hPathUpdate(float)),    pfdC,   SLOT(setDevH(float)));
   //connect(sb, SIGNAL(vPathUpdate(float)),    pfdC,   SLOT(setDevV(float)));
   
   connect(sb, SIGNAL(slipSkidUpdate(float)), pfdC,   SLOT(setSlipSkid(float)));
   connect(sb, SIGNAL(slipSkidUpdate(float)), tcdC,   SLOT(setSlipSkid(float)));

   // Compass != Heading
   /*
   connect(sb, SIGNAL(compassUpdate(float)), mapView, SLOT(setHeading(float)));
   connect(sb, SIGNAL(compassUpdate(float)), overlay, SLOT(setHeading(float)));
   connect(sb, SIGNAL(compassUpdate(float)), pfdC,    SLOT(setHeading(float)));
   connect(sb, SIGNAL(compassUpdate(float)), hsiC,    SLOT(setHeading(float)));
   // */

   // Position (this AC)
   connect(sb, SIGNAL(latLonUpdate(float, float)), window, SLOT(latLonUpdate(float, float)));

   // Altitudes: using MSL, but AGL could be connected later
   connect(sb, SIGNAL(altMSLUpdate(float)), pfdC,     SLOT(setAltitude(float)));
   connect(sb, SIGNAL(altMSLUpdate(float)), altC,     SLOT(setAltitude(float)));

   //connect(sb, SIGNAL(altAGLUpdate(float)), pfdC,     SLOT(setAltitude(float)));
   //connect(sb, SIGNAL(altAGLUpdate(float)), altC,     SLOT(setAltitude(float)));

   // Position (other AC)
   connect(sb, SIGNAL(acLatUpdate(float, int)), this, SLOT(updateACLat(float, int)));
   connect(sb, SIGNAL(acLonUpdate(float, int)), this, SLOT(updateACLon(float, int)));
   connect(sb, SIGNAL(acAltUpdate(float, int)), this, SLOT(updateACAlt(float, int)));
   
   // Comms and Navs
   connect(sb, SIGNAL(com1Update(float, float)), comW, SLOT(setCom1(float, float)));
   connect(sb, SIGNAL(com2Update(float, float)), comW, SLOT(setCom2(float, float)));
   //connect(sb, SIGNAL(comTransmitUpdate(float), commsWidget, SLOT(setComTransmit(float))));
   
   connect(sb, SIGNAL(nav1Update(float, float)), comW, SLOT(setNav1(float, float)));
   connect(sb, SIGNAL(nav2Update(float, float)), comW, SLOT(setNav2(float, float)));
}


float HDDController::calculateTurnRate(float q, float r, float pitch, float roll)
{
   // Calculate Turn Rate in rad/s
   float turnRateRPS = ( 1/cos(deg2rad(pitch)) ) * ( sin(deg2rad(roll)) * q + cos(deg2rad(roll)) * r );
   float turnRateDPS = turnRateRPS * 180/PI;
   return turnRateDPS;
}

void HDDController::updateAngVel(float q, float p, float r)
{
   angVelQ = q;
   angVelP = p;
   angVelR = r;
   angVelUpdatedFlag = true;
   tryCalculateTurnRate();
}

void HDDController::updatePitch(float p)
{
   pitch = p;
   pitchUpdatedFlag = true;
   tryCalculateTurnRate();
}

void HDDController::updateRoll(float r)
{
   roll = r;
   rollUpdatedFlag = true;
   tryCalculateTurnRate();
}

/*
 * Attempts to update the turn rate, but only works if all 3 flags are true.
 */
void HDDController::tryCalculateTurnRate()
{
   if (angVelUpdatedFlag && pitchUpdatedFlag && rollUpdatedFlag) {
      float turnRate = calculateTurnRate(angVelQ, angVelR, pitch, roll);
      emit turnRateUpdate(turnRate);
      // Reset the flags
      angVelUpdatedFlag = false;
      pitchUpdatedFlag = false;
      rollUpdatedFlag = false;
   }
}

void HDDController::updateACLat(float lat, int ac)
{
   // If this aircraft has not been identified yet, add it to the list
   if (!acMap->contains(ac)) {
      Aircraft* a = new Aircraft(ac, this);
      a->setLatLonAlt(lat, 0.0, 0.0);
      connect(a, SIGNAL(acUpdated(int)), this, SLOT(acUpdated(int)));
      acMap->insert(ac, a);
      return;
   }
   Aircraft* a = acMap->value(ac);
   a->setLat(lat);
   acMap->insert(ac, a); // update the map
}

void HDDController::updateACLon(float lon, int ac)
{
   // If this aircraft has not been identified yet, add it to the list
   if (!acMap->contains(ac)) {
      Aircraft* a = new Aircraft(ac, this);
      a->setLatLonAlt(0.0, lon, 0.0);
      connect(a, SIGNAL(acUpdated(int)), this, SLOT(acUpdated(int)));
      acMap->insert(ac, a);
      return;
   }
   Aircraft* a = acMap->value(ac);
   a->setLon(lon);
   acMap->insert(ac, a); // update the map
}

void HDDController::updateACAlt(float alt, int ac)
{
   // If this aircraft has not been identified yet, add it to the list
   if (!acMap->contains(ac)) {
      Aircraft* a = new Aircraft(ac, this);
      a->setLatLonAlt(0.0, 0.0, alt);
      connect(a, SIGNAL(acUpdated(int)), this, SLOT(acUpdated(int)));
      acMap->insert(ac, a);
      return;
   }
   Aircraft* a = acMap->value(ac);
   a->setAlt(alt);
   acMap->insert(ac, a); // update the map
}

/*
 * This slot is called when any given aircraft (identified by id) has had its
 * lat, lon and altitude updated.
 * 
 * Update the map so it can draw the AC.
 * Update the TFC controller so it can update the displayed values if this AC
 * is currently selected.
 */
void HDDController::acUpdated(int id)
{
   TrafficController* tfcC = window->getTfcC();
   tfcC->setDisplayedAC(acMap->value(id));
   
   // TODO: update the map for drawing the AC
}
