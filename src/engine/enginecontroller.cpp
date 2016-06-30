/*
 * File:   enginecontroller.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 23, 2015, 12:33 PM
 */

#include "enginecontroller.h"

#include <QDebug>

#include "core/cpdsettings.h"
#include "core/switchboard.h"


EngineController::EngineController(
  CPDSettings* _cpdSettings,
  SwitchBoard* _sb,
  AircraftType _engType,
  int _numEngines,
  int _numFuelTanks,
  QObject* _parent
) : QObject(_parent),
cpdSettings(_cpdSettings),
sb(_sb),
engType(_engType),
numEngines(_numEngines),
numFuelTanks(_numFuelTanks) {
  connect(sb, &SWB::acTypeUpdate,         this, &ENGC::updateACType);
  connect(sb, &SWB::acNumEnginesUpdate,   this, &ENGC::updateNumEngines);

  // Engine limits
  connect(sb, &SWB::engLimitMPUpdate,     this, &ENGC::engLimitMPUpdate);
  connect(sb, &SWB::engLimitFFUpdate,     this, &ENGC::engLimitFFUpdate);
  connect(sb, &SWB::engLimitN1Update,     this, &ENGC::engLimitN1Update);
  connect(sb, &SWB::engLimitN2Update,     this, &ENGC::engLimitN2Update);

  connect(sb, &SWB::engLimitEPRUpdate,    this, &ENGC::engLimitEPRUpdate);
  connect(sb, &SWB::engLimitEGTUpdate,    this, &ENGC::engLimitEGTUpdate);
  connect(sb, &SWB::engLimitTRQUpdate,    this, &ENGC::engLimitTRQUpdate);
  connect(sb, &SWB::engLimitITTUpdate,    this, &ENGC::engLimitITTUpdate);
  connect(sb, &SWB::engLimitCHTUpdate,    this, &ENGC::engLimitCHTUpdate);

  connect(sb, &SWB::engLimitOilPUpdate,   this, &ENGC::engLimitOilPUpdate);
  connect(sb, &SWB::engLimitOilTUpdate,   this, &ENGC::engLimitOilTUpdate);
  connect(sb, &SWB::engLimitFuelPUpdate,  this, &ENGC::engLimitFuelPUpdate);

  // Throttle settings and actual values
  connect(sb, &SWB::throttleCommandUpdate,  this, &ENGC::updateThrottleCommand);
  connect(sb, &SWB::throttleActualUpdate,   this, &ENGC::updateThrottleActual);

  // Engine settings
  connect(sb, &SWB::engPowerUpdate,    this, &ENGC::engPowerUpdate);
  connect(sb, &SWB::engThrustUpdate,   this, &ENGC::engThrustUpdate);
  connect(sb, &SWB::engTorqueUpdate,   this, &ENGC::engTorqueUpdate);
  connect(sb, &SWB::engRPMUpdate,      this, &ENGC::engRPMUpdate);
  connect(sb, &SWB::propRPMUpdate,     this, &ENGC::propRPMUpdate);
  connect(sb, &SWB::propPitchUpdate,   this, &ENGC::propPitchUpdate);
  connect(sb, &SWB::propwashUpdate,    this, &ENGC::propwashUpdate);

  connect(sb, &SWB::n1Update,    this, &ENGC::n1Update);
  connect(sb, &SWB::n2Update,    this, &ENGC::n2Update);
  connect(sb, &SWB::mpUpdate,    this, &ENGC::mpUpdate);
  connect(sb, &SWB::eprUpdate,   this, &ENGC::eprUpdate);
  connect(sb, &SWB::ffUpdate,    this, &ENGC::ffUpdate);
  connect(sb, &SWB::ittUpdate,   this, &ENGC::ittUpdate);
  connect(sb, &SWB::egtUpdate,   this, &ENGC::egtUpdate);
  connect(sb, &SWB::chtUpdate,   this, &ENGC::chtUpdate);

  connect(sb, &SWB::engOilPressureUpdate, this, &ENGC::oilPUpdate);
  connect(sb, &SWB::engOilTempUpdate,     this, &ENGC::oilTUpdate);
}

//EngineController::EngineController(const EngineController& orig) {
//}

EngineController::~EngineController() {
}


void EngineController::updateACType(int acType) {
  AircraftType type = (AircraftType) acType;
  engType = (AircraftType) (acType & ENG_TYPES);
  qDebug() << "Engine type:" << engType;

  emit engineTypeUpdate(engType);
}

void EngineController::updateNumEngines(int numEng) {
  // qDebug() << "Updating num engines...";
  numEngines = numEng;
  emit numEnginesUpdate(numEngines);
}

void EngineController::updateThrottleCommand(float throttle, int engNum) {
  // If this throttle is unknown, or if the throttle is 0.75 (the default if it does not exist)
  if (engNum > numEngines-1 || throttle == 0.75) {
//    QProgressBar* throttle = createThrottle(100, (int) throttle);
//    throttles.append(throttle);
    return;
  }
  emit throttleUpdate((int) 100*throttle, engNum);
}

void EngineController::updateThrottleActual(float throttle, int engNum) {
}
