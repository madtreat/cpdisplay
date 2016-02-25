/* 
 * File:   cpdcontroller.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 15, 2015, 6:39 PM
 */

#include "cpdcontroller.h"

#include <math.h>

#include "core/cpdsettings.h"
#include "core/switchboard.h"
#include "utils/geodist.h"

#include "instruments/adicontroller.h"
#include "instruments/altcontroller.h"
#include "instruments/asicontroller.h"
#include "instruments/hsicontroller.h"
#include "instruments/pfdcontroller.h"
#include "instruments/tcdcontroller.h"
#include "instruments/vsicontroller.h"

#include "map/mapcontroller.h"
#include "gear/gearcontroller.h"
#include "comms/commscontroller.h"
#include "engine/enginecontroller.h"
#include "traffic/trafficcontroller.h"


CPDController::CPDController(
  CPDSettings* _settings,
  int _slaveID,
  QObject* _parent
)
: QObject(_parent),
slaveID(_slaveID) {
  settings = _settings;
  acMap = new QMap<int, Aircraft*>();

  sb = new SwitchBoard(settings, slaveID);

  mapC = new MapController(settings->mapSettingsFile(), sb, acMap, this);
  adiC = new ADIController(this);
  altC = new ALTController(this);
  asiC = new ASIController(this);
  hsiC = new HSIController(this);
  pfdC = new PFDController(this);
  tcdC = new TCDController(this);
  vsiC = new VSIController(this);

  numEngines = 2;
  numFuelTanks = 2;
  numGears = 3;
  gearC = new GearController(settings, sb, numGears, this);
  comC = new CommsController(settings, sb, this);
  engC = new EngineController(settings, AC_ENG_JET, numEngines, numFuelTanks, this);
  tfcC = new TrafficController(settings, sb, acMap, this);

  angVelUpdatedFlag = false;
  pitchUpdatedFlag = false;
  rollUpdatedFlag = false;

  connectSignals();

//  window->show();
}

//CPDController::CPDController(const CPDController& orig) {
//}

CPDController::~CPDController() {
}

void CPDController::printSomething(float value) {
  qDebug() << "CPDController received a value:" << value << "from AOA_Derived";
}


/*
 * Connects the switchboard directly to the widgets.
 */
void CPDController::connectSignals() {
  //  EngineWidget* engW = engC->getWidget();
  //  TrafficWidget* tfcW = tfcC->getWidget();

  /*
   * These connections are for xplane 10.40+ dataref requests (RREF results).
   */
  // General aircraft
  // connect(sb, &SWB::acTailNumUpdate,     this, &CPDC::setTailNum);
  connect(sb, &SWB::acNumEnginesUpdate,    engC,  &ENGC::updateNumEngines);
  connect(sb, &SWB::gearRetractableUpdate, this,  &CPDC::updateGearRetractable);

  // Engine limits
  connect(sb, &SWB::engLimitMPUpdate,    engC, &ENGC::engLimitMPUpdate);
  connect(sb, &SWB::engLimitFFUpdate,    engC, &ENGC::engLimitFFUpdate);
  connect(sb, &SWB::engLimitN1Update,    engC, &ENGC::engLimitN1Update);
  connect(sb, &SWB::engLimitN2Update,    engC, &ENGC::engLimitN2Update);

  connect(sb, &SWB::engLimitEPRUpdate,   engC, &ENGC::engLimitEPRUpdate);
  connect(sb, &SWB::engLimitEGTUpdate,   engC, &ENGC::engLimitEGTUpdate);
  connect(sb, &SWB::engLimitTRQUpdate,   engC, &ENGC::engLimitTRQUpdate);
  connect(sb, &SWB::engLimitITTUpdate,   engC, &ENGC::engLimitITTUpdate);
  connect(sb, &SWB::engLimitCHTUpdate,   engC, &ENGC::engLimitCHTUpdate);

  connect(sb, &SWB::engLimitOilPUpdate,  engC, &ENGC::engLimitOilPUpdate);
  connect(sb, &SWB::engLimitOilTUpdate,  engC, &ENGC::engLimitOilTUpdate);
  connect(sb, &SWB::engLimitFuelPUpdate, engC, &ENGC::engLimitFuelPUpdate);

  // Multiplayer: other aircraft
  connect(sb, &SWB::acHdgUpdate,         this,  &CPDC::updateACHdg);
  connect(sb, &SWB::acSpdXUpdate,        this,  &CPDC::updateACSpdX);
  connect(sb, &SWB::acSpdYUpdate,        this,  &CPDC::updateACSpdY);
  connect(sb, &SWB::acSpdZUpdate,        this,  &CPDC::updateACSpdZ);

  // Misc:
  connect(sb, &SWB::aoaDUpdate,          this, &CPDC::printSomething);


  /*
   * These connections are for xplane 10.40+ dataref set requests (DREFs sent
   * to xplane).
   */


  /*
   * Everything below is for the raw UDP output from xplane, if selected.
   */
  // Self-calculated turn rate
  connect(this, &CPDC::turnRateUpdate, pfdC, &PFDC::setTurnRate);
  connect(this, &CPDC::turnRateUpdate, tcdC, &TCDC::setTurnRate);

  // Speeds
  connect(sb, &SWB::speedUpdate,   asiC,    &ASIC::setAirspeed);
  connect(sb, &SWB::speedUpdate,   pfdC,    &PFDC::setAirspeed);

  // Mach, Vertical Velocity
  connect(sb, &SWB::machNumUpdate, pfdC,    &PFDC::setMachNo);

  connect(sb, &SWB::vertVelUpdate, pfdC,    &PFDC::setClimbRate);
  connect(sb, &SWB::vertVelUpdate, vsiC,    &VSIC::setClimbRate);

  // Pressure
  connect(sb, &SWB::pressureUpdate, altC,   &ALTC::setPressure);
  connect(sb, &SWB::pressureUpdate, pfdC,   &PFDC::setPressure);

  // Angular Velocities (Q, P, R)
  connect(sb, &SWB::angVelUpdate, this, &CPDC::updateAngVel);

  // Pitch, Roll, Heading
  connect(sb, &SWB::pitchUpdate,   this,    &CPDC::updatePitch);
  connect(sb, &SWB::pitchUpdate,   adiC,    &ADIC::setPitch);
  connect(sb, &SWB::pitchUpdate,   pfdC,    &PFDC::setPitch);

  connect(sb, &SWB::rollUpdate,    this,    &CPDC::updateRoll);
  connect(sb, &SWB::rollUpdate,    adiC,    &ADIC::setRoll);
  connect(sb, &SWB::rollUpdate,    pfdC,    &PFDC::setRoll);

  connect(sb, &SWB::headingMagUpdate, hsiC, &HSIC::setHeading);
  connect(sb, &SWB::headingMagUpdate, pfdC, &PFDC::setHeading);

  // AOA, SideSlip
  connect(sb, SIGNAL(aoaSideSlipUpdate(float, float)), pfdC, SLOT(setFlightPathMarker(float, float)));

  // these are not the values
  //connect(sb, &SWB::hPathUpdate,    pfdC,   &PFDC::setDevH);
  //connect(sb, &SWB::vPathUpdate,    pfdC,   &PFDC::setDevV);

  connect(sb, &SWB::slipSkidUpdate, pfdC,   &PFDC::setSlipSkid);
  connect(sb, &SWB::slipSkidUpdate, tcdC,   &TCDC::setSlipSkid);

  // Compass != Heading
  /*
  connect(sb, &SWB::compassUpdate, mapC,    &MAPC::setHeading);
  connect(sb, &SWB::compassUpdate, pfdC,    &PFDC::setHeading);
  connect(sb, &SWB::compassUpdate, hsiC,    &HSIC::setHeading);
  // */

  // Altitudes: using MSL, but AGL could be connected later
  connect(sb, &SWB::altMSLUpdate, pfdC,     &PFDC::setAltitude);
  connect(sb, &SWB::altMSLUpdate, altC,     &ALTC::setAltitude);

  //connect(sb, &SWB::altAGLUpdate, pfdC,     &PFDC::setAltitude);
  //connect(sb, &SWB::altAGLUpdate, altC,     &ALTC::setAltitude);

  // Position (other AC)
  connect(sb, &SWB::acLatUpdate, this, &CPDC::updateACLat);
  connect(sb, &SWB::acLonUpdate, this, &CPDC::updateACLon);
  connect(sb, &SWB::acAltUpdate, this, &CPDC::updateACAlt);

  // Throttle settings and actual values
  connect(sb, &SWB::throttleCommandUpdate,   engC, &ENGC::updateThrottleCommand);
  connect(sb, &SWB::throttleActualUpdate,    engC, &ENGC::updateThrottleActual);

  // Engine settings
  connect(sb, &SWB::engPowerUpdate,    engC, &ENGC::updateEngPower);
  connect(sb, &SWB::engThrustUpdate,   engC, &ENGC::updateEngThrust);
  connect(sb, &SWB::engTorqueUpdate,   engC, &ENGC::updateEngTorque);
  connect(sb, &SWB::engRPMUpdate,      engC, &ENGC::updateEngRPM);
  connect(sb, &SWB::propRPMUpdate,     engC, &ENGC::updatePropRPM);
  connect(sb, &SWB::propPitchUpdate,   engC, &ENGC::updatePropPitch);
  connect(sb, &SWB::propwashUpdate,    engC, &ENGC::updatePropwash);

  connect(sb, &SWB::n1Update,    engC, &ENGC::updateN1);
  connect(sb, &SWB::n2Update,    engC, &ENGC::updateN2);
  connect(sb, &SWB::mpUpdate,    engC, &ENGC::updateMP);
  connect(sb, &SWB::eprUpdate,   engC, &ENGC::updateEPR);
  connect(sb, &SWB::ffUpdate,    engC, &ENGC::updateFF);
  connect(sb, &SWB::ittUpdate,   engC, &ENGC::updateITT);
  connect(sb, &SWB::egtUpdate,   engC, &ENGC::updateEGT);
  connect(sb, &SWB::chtUpdate,   engC, &ENGC::updateCHT);

  connect(sb, &SWB::engOilPressureUpdate, engC, &ENGC::updateOilPressure);
  connect(sb, &SWB::engOilTempUpdate,     engC, &ENGC::updateOilTemp);
}


float CPDController::calculateTurnRate(
  float q,
  float r,
  float pitch,
  float roll
) {
  // Calculate Turn Rate in rad/s
  float turnRateRPS = ( 1/cos(deg2rad(pitch)) ) * ( sin(deg2rad(roll)) * q + cos(deg2rad(roll)) * r );
  float turnRateDPS = turnRateRPS * 180/PI;
  return turnRateDPS;
}

Aircraft* CPDController::createAircraft(
  int id,
  float lat,
  float lon,
  float alt
) {
  Aircraft* a = new Aircraft(id, this);
  a->setLatLonAlt(lat, lon, alt);
  connect(a, &Aircraft::acUpdated,  tfcC, &TFCC::acUpdated);
  connect(a, &Aircraft::acUpdated,  mapC, &MAPC::acUpdated);
//  connect(a, &Aircraft::rngBerUpdated, mapC, &MAPC::updateACPos);
  acMap->insert(id, a);
  return a;
}

void CPDController::updateAngVel(float q, float p, float r) {
  angVelQ = q;
  angVelP = p;
  angVelR = r;
  angVelUpdatedFlag = true;
  tryCalculateTurnRate();
}

void CPDController::updatePitch(float p) {
  pitch = p;
  pitchUpdatedFlag = true;
  tryCalculateTurnRate();
}

void CPDController::updateRoll(float r) {
  roll = r;
  rollUpdatedFlag = true;
  tryCalculateTurnRate();
}

/*
 * Attempts to update the turn rate, but only works if all 3 flags are true.
 */
void CPDController::tryCalculateTurnRate() {
  if (angVelUpdatedFlag && pitchUpdatedFlag && rollUpdatedFlag) {
    float turnRate = calculateTurnRate(angVelQ, angVelR, pitch, roll);
    emit turnRateUpdate(turnRate);
    // Reset the flags
    angVelUpdatedFlag = false;
    pitchUpdatedFlag = false;
    rollUpdatedFlag = false;
  }
}

void CPDController::updateACLat(float lat, int ac) {
  // If this aircraft has not been identified yet, add it to the list
  if (!acMap->contains(ac)) {
    createAircraft(ac, lat, 0.0, 0.0);
    return;
  }
  Aircraft* a = acMap->value(ac);
  a->setLat(lat);
  acMap->insert(ac, a); // update the map
}

void CPDController::updateACLon(float lon, int ac) {
  // If this aircraft has not been identified yet, add it to the list
  if (!acMap->contains(ac)) {
    createAircraft(ac, 0.0, lon, 0.0);
    return;
  }
  Aircraft* a = acMap->value(ac);
  a->setLon(lon);
  acMap->insert(ac, a); // update the map
}

void CPDController::updateACAlt(float alt, int ac) {
  // If this aircraft has not been identified yet, add it to the list
  if (!acMap->contains(ac)) {
    createAircraft(ac, 0.0, 0.0, alt);
    return;
  }
  Aircraft* a = acMap->value(ac);
  a->setAlt(alt);
  acMap->insert(ac, a); // update the map
}

void CPDController::updateACHdg(float hdg, int ac) {
  // If this aircraft has not been identified yet, add it to the list
  if (!acMap->contains(ac)) {
    // createAircraft(ac, 0.0, 0.0, alt);
    return;
  }
  Aircraft* a = acMap->value(ac);
  a->setHdg(hdg);
  acMap->insert(ac, a); // update the map
}

void CPDController::updateACSpdX(float vx, int ac) {
  // If this aircraft has not been identified yet, add it to the list
  if (!acMap->contains(ac)) {
    // createAircraft(ac, 0.0, 0.0, alt);
    return;
  }
  Aircraft* a = acMap->value(ac);
  a->setSpdX(vx);
  acMap->insert(ac, a); // update the map
}

void CPDController::updateACSpdY(float vy, int ac) {
  // If this aircraft has not been identified yet, add it to the list
  if (!acMap->contains(ac)) {
    // createAircraft(ac, 0.0, 0.0, alt);
    return;
  }
  Aircraft* a = acMap->value(ac);
  a->setSpdY(vy);
  acMap->insert(ac, a); // update the map
}

void CPDController::updateACSpdZ(float vz, int ac) {
  // If this aircraft has not been identified yet, add it to the list
  if (!acMap->contains(ac)) {
    // createAircraft(ac, 0.0, 0.0, alt);
    return;
  }
  Aircraft* a = acMap->value(ac);
  a->setSpdZ(vz);
  acMap->insert(ac, a); // update the map
}
