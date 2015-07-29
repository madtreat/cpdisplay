/* 
 * File:   hddcontroller.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 15, 2015, 6:39 PM
 */

#include "hddcontroller.h"

#include <math.h>

#include "core/hddsettings.h"
#include "core/switchboard.h"
#include "utils/geodist.h"

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
//   window = new HDDWindow(settings, acMap, this);
   
   mapC = new MapController(settings, acMap, this);
   adiC = new ADIController(this);
   altC = new ALTController(this);
   asiC = new ASIController(this);
   hsiC = new HSIController(this);
   pfdC = new PFDController(this);
   tcdC = new TCDController(this);
   vsiC = new VSIController(this);
   
   numEngines = 2;
   comC = new CommsController(settings, this);
   engC = new EngineController(settings, numEngines, this);
   tfcC = new TrafficController(settings, acMap, this);

   angVelUpdatedFlag = false;
   pitchUpdatedFlag = false;
   rollUpdatedFlag = false;

   connectSignals();

//   window->show();
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
   MapView*    mapView = getMapView();
   MapOverlay* overlay = getOverlay();
   
   EngineWidget* engW = engC->getWidget();
//   TrafficWidget* tfcW = tfcC->getWidget();


   /*
    * These connections are for xplane 10.40+ dataref requests (RREF results).
    */
   connect(sb, SIGNAL(acTailNumUpdate(float)), this, SLOT(setTailNum(float)));
   connect(sb, SIGNAL(acNumEnginesUpdate(float)), this, SLOT(setNumEngines(float)));

   /*
    * Everything below is for the raw UDP output from xplane, if selected.
    */
   // Self-calculated turn rate
   connect(this, SIGNAL(turnRateUpdate(float)), pfdC, SLOT(setTurnRate(float)));
   connect(this, SIGNAL(turnRateUpdate(float)), tcdC, SLOT(setTurnRate(float)));
   
   // Times
   connect(sb, SIGNAL(timeUpdate(float, float)), comC, SLOT(setTimes(float, float)));

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
   connect(sb, SIGNAL(latLonUpdate(float, float)), mapC, SLOT(panToLocation(float, float)));

   // Altitudes: using MSL, but AGL could be connected later
   connect(sb, SIGNAL(altMSLUpdate(float)), pfdC,     SLOT(setAltitude(float)));
   connect(sb, SIGNAL(altMSLUpdate(float)), altC,     SLOT(setAltitude(float)));

   //connect(sb, SIGNAL(altAGLUpdate(float)), pfdC,     SLOT(setAltitude(float)));
   //connect(sb, SIGNAL(altAGLUpdate(float)), altC,     SLOT(setAltitude(float)));

   // Position (other AC)
   connect(sb, SIGNAL(acLatUpdate(float, int)), this, SLOT(updateACLat(float, int)));
   connect(sb, SIGNAL(acLonUpdate(float, int)), this, SLOT(updateACLon(float, int)));
   connect(sb, SIGNAL(acAltUpdate(float, int)), this, SLOT(updateACAlt(float, int)));
   
   // Throttle settings and actual values
   connect(sb, SIGNAL(throttleCommandUpdate(float, int)), engW, SLOT(updateThrottleCommand(float, int)));
   connect(sb, SIGNAL(throttleActualUpdate(float, int)), engW, SLOT(updateThrottleActual(float, int)));
   
   // Engine settings
   connect(sb, SIGNAL(engPowerUpdate(float, int)), engW, SLOT(updateEngPower(float, int)));
   connect(sb, SIGNAL(engThrustUpdate(float, int)), engW, SLOT(updateEngThrust(float, int)));
   connect(sb, SIGNAL(engTorqueUpdate(float, int)), engW, SLOT(updateEngTorque(float, int)));
   connect(sb, SIGNAL(engRPMUpdate(float, int)), engW, SLOT(updateEngRPM(float, int)));
   
   // Comms and Navs
   connect(sb, SIGNAL(com1Update(float, float)), comC, SLOT(setCom1(float, float)));
   connect(sb, SIGNAL(com2Update(float, float)), comC, SLOT(setCom2(float, float)));
   //connect(sb, SIGNAL(comTransmitUpdate(float), commsWidget, SLOT(setComTransmit(float))));
   
   connect(sb, SIGNAL(nav1Update(float, float)), comC, SLOT(setNav1(float, float)));
   connect(sb, SIGNAL(nav2Update(float, float)), comC, SLOT(setNav2(float, float)));
}


float HDDController::calculateTurnRate(float q, float r, float pitch, float roll)
{
   // Calculate Turn Rate in rad/s
   float turnRateRPS = ( 1/cos(deg2rad(pitch)) ) * ( sin(deg2rad(roll)) * q + cos(deg2rad(roll)) * r );
   float turnRateDPS = turnRateRPS * 180/PI;
   return turnRateDPS;
}

Aircraft* HDDController::createAircraft(int id, float lat, float lon, float alt)
{
   Aircraft* a = new Aircraft(id, this);
   a->setLatLonAlt(lat, lon, alt);
   connect(a, SIGNAL(acUpdated(int)), tfcC, SLOT(acUpdated(int)));
   connect(a, SIGNAL(acUpdated(int)), mapC, SLOT(acUpdated(int)));
//   connect(a, SIGNAL(rngBerUpdated(int)), mapC, SLOT(updateACPos(int)));
   acMap->insert(id, a);
   return a;
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
      createAircraft(ac, lat, 0.0, 0.0);
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
      createAircraft(ac, 0.0, lon, 0.0);
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
      createAircraft(ac, 0.0, 0.0, alt);
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
   
   // TODO: update the map for drawing the AC
}
