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
  AircraftEngineType _engType,
  int _numEngines,
  int _numFuelTanks,
  QObject* _parent
) : QObject(_parent),
cpdSettings(_cpdSettings),
sb(_sb),
engType(_engType),
numEngines(_numEngines),
numFuelTanks(_numFuelTanks) {
  connect(sb, &SWB::acNumEnginesUpdate,    this,  &ENGC::updateNumEngines);

  // Engine limits
  connect(sb, &SWB::engLimitMPUpdate,    this, &ENGC::engLimitMPUpdate);
  connect(sb, &SWB::engLimitFFUpdate,    this, &ENGC::engLimitFFUpdate);
  connect(sb, &SWB::engLimitN1Update,    this, &ENGC::engLimitN1Update);
  connect(sb, &SWB::engLimitN2Update,    this, &ENGC::engLimitN2Update);

  connect(sb, &SWB::engLimitEPRUpdate,   this, &ENGC::engLimitEPRUpdate);
  connect(sb, &SWB::engLimitEGTUpdate,   this, &ENGC::engLimitEGTUpdate);
  connect(sb, &SWB::engLimitTRQUpdate,   this, &ENGC::engLimitTRQUpdate);
  connect(sb, &SWB::engLimitITTUpdate,   this, &ENGC::engLimitITTUpdate);
  connect(sb, &SWB::engLimitCHTUpdate,   this, &ENGC::engLimitCHTUpdate);

  connect(sb, &SWB::engLimitOilPUpdate,  this, &ENGC::engLimitOilPUpdate);
  connect(sb, &SWB::engLimitOilTUpdate,  this, &ENGC::engLimitOilTUpdate);
  connect(sb, &SWB::engLimitFuelPUpdate, this, &ENGC::engLimitFuelPUpdate);

  // Throttle settings and actual values
  connect(sb, &SWB::throttleCommandUpdate,   this, &ENGC::updateThrottleCommand);
  connect(sb, &SWB::throttleActualUpdate,    this, &ENGC::updateThrottleActual);

  // Engine settings
  connect(sb, &SWB::engPowerUpdate,    this, &ENGC::updateEngPower);
  connect(sb, &SWB::engThrustUpdate,   this, &ENGC::updateEngThrust);
  connect(sb, &SWB::engTorqueUpdate,   this, &ENGC::updateEngTorque);
  connect(sb, &SWB::engRPMUpdate,      this, &ENGC::updateEngRPM);
  connect(sb, &SWB::propRPMUpdate,     this, &ENGC::updatePropRPM);
  connect(sb, &SWB::propPitchUpdate,   this, &ENGC::updatePropPitch);
  connect(sb, &SWB::propwashUpdate,    this, &ENGC::updatePropwash);

  connect(sb, &SWB::n1Update,    this, &ENGC::updateN1);
  connect(sb, &SWB::n2Update,    this, &ENGC::updateN2);
  connect(sb, &SWB::mpUpdate,    this, &ENGC::updateMP);
  connect(sb, &SWB::eprUpdate,   this, &ENGC::updateEPR);
  connect(sb, &SWB::ffUpdate,    this, &ENGC::updateFF);
  connect(sb, &SWB::ittUpdate,   this, &ENGC::updateITT);
  connect(sb, &SWB::egtUpdate,   this, &ENGC::updateEGT);
  connect(sb, &SWB::chtUpdate,   this, &ENGC::updateCHT);

  connect(sb, &SWB::engOilPressureUpdate, this, &ENGC::updateOilPressure);
  connect(sb, &SWB::engOilTempUpdate,     this, &ENGC::updateOilTemp);
}

//EngineController::EngineController(const EngineController& orig) {
//}

EngineController::~EngineController() {
}

void EngineController::updateEngineType(AircraftEngineType type) {
  engType = type;
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

void EngineController::updateEngPower(float power, int engNum) {
}

void EngineController::updateEngThrust(float thrust, int engNum) {
}

void EngineController::updateEngTorque(float torque, int engNum) {
}

void EngineController::updateEngRPM(float rpm, int engNum) {
  emit engRPMUpdate(rpm, engNum);
}

void EngineController::updatePropRPM(float rpm, int engNum) {
  emit propRPMUpdate(rpm, engNum);
}

void EngineController::updatePropPitch(float pitch, int engNum) {
  emit propPitchUpdate(pitch, engNum);
}

void EngineController::updatePropwash(float pwash, int engNum) {
  emit propwashUpdate(pwash, engNum);
}

void EngineController::updateN1(float n1, int engNum) {
  emit n1Update(n1, engNum);
}

void EngineController::updateN2(float n2, int engNum) {
  emit n2Update(n2, engNum);
}

void EngineController::updateMP(float mp, int engNum) {
  emit mpUpdate(mp, engNum);
}

void EngineController::updateEPR(float epr, int engNum) {
  emit eprUpdate(epr, engNum);
}

void EngineController::updateFF(float ff, int engNum) {
  emit ffUpdate(ff, engNum);
}

void EngineController::updateITT(float itt, int engNum) {
  emit ittUpdate(itt, engNum);
}

void EngineController::updateEGT(float egt, int engNum) {
  emit egtUpdate(egt, engNum);
}

void EngineController::updateCHT(float cht, int engNum) {
  emit chtUpdate(cht, engNum);
}

void EngineController::updateOilPressure(float pressure, int engNum) {
  emit oilPUpdate(pressure, engNum);
}

void EngineController::updateOilTemp(float temp, int engNum) {
  emit oilTUpdate(temp, engNum);
}

void EngineController::updateFuelQuantity(float fuel, int engNum) {
  emit fuelUpdate(fuel, engNum);
}
