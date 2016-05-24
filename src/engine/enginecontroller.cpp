/*
 * File:   enginecontroller.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 23, 2015, 12:33 PM
 */

#include "enginecontroller.h"

#include <QDebug>

#include "core/cpdsettings.h"


EngineController::EngineController(
  CPDSettings* _cpdSettings,
  AircraftEngineType _engType,
  int _numEngines,
  int _numFuelTanks,
  QObject* _parent
) : QObject(_parent),
cpdSettings(_cpdSettings),
engType(_engType),
numEngines(_numEngines),
numFuelTanks(_numFuelTanks) {
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
