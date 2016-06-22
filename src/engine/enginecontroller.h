/*
 * File:   enginecontroller.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 23, 2015, 12:33 PM
 */

#ifndef ENGINECONTROLLER_H
#define ENGINECONTROLLER_H

#include <QObject>

#include "core/aircraft.h"
#include "core/xplanedref.h"
#include "enginewidget.h"


class SwitchBoard;
class CPDSettings;
class EngineWidget;

class EngineController : public QObject {
  Q_OBJECT;

public:
  EngineController(CPDSettings* _cpdSettings, SwitchBoard* _sb, AircraftEngineType _engType, int _numEngines, int _numFuelTanks, QObject* _parent = 0);
  EngineController(const EngineController& orig) = delete;
  virtual ~EngineController();

  AircraftEngineType getEngineType() const { return engType; }

public slots:
  void updateEngineType(AircraftEngineType type);
  void updateNumEngines(int numEng);
  void updateThrottleCommand(float throttle, int engNum);
  void updateThrottleActual(float throttle, int engNum);
  void updateEngPower(float power, int engNum);
  void updateEngThrust(float thrust, int engNum);
  void updateEngTorque(float torque, int engNum);
  void updateEngRPM(float rpm, int engNum);
  void updatePropRPM(float rpm, int engNum);
  void updatePropPitch(float pitch, int engNum);
  void updatePropwash(float pwash, int engNum);

  void updateN1(float n1, int engNum);
  void updateN2(float n2, int engNum);
  void updateMP(float mp, int engNum);
  void updateEPR(float epr, int engNum);
  void updateFF(float ff, int engNum);
  void updateITT(float itt, int engNum);
  void updateEGT(float egt, int engNum);
  void updateCHT(float cht, int engNum);

  void updateOilPressure(float pressure, int engNum);
  void updateOilTemp(float temp, int engNum);

  void updateFuelQuantity(float fuel, int engNum);

signals:
  void engineTypeUpdate(AircraftEngineType type);
  void numEnginesUpdate(int numEngines);
  void numTanksUpdate(int numTanks);

  void throttleUpdate(int value, int engNum);
  void engRPMUpdate(float value, int engNum);
  void propRPMUpdate(float value, int engNum);
  void propPitchUpdate(float value, int engNum);
  void propwashUpdate(float value, int engNum);

  void n1Update(float value, int engNum);
  void n2Update(float value, int engNum);
  void mpUpdate(float value, int engNum);
  void eprUpdate(float value, int engNum);
  void ffUpdate(float value, int engNum);
  void ittUpdate(float value, int engNum);
  void egtUpdate(float value, int engNum);
  void chtUpdate(float value, int engNum);

  void oilPUpdate(float value, int engNum);
  void oilTUpdate(float value, int engNum);

  void fuelUpdate(float value, int engNum);

  // Engine limit signals, passed through from CPDController
  void engLimitMPUpdate(float value, LimitType type);     // in hg
  void engLimitFFUpdate(float value, LimitType type);     // gal/hr
  void engLimitN1Update(float value, LimitType type);     // percent
  void engLimitN2Update(float value, LimitType type);     // percent

  void engLimitEPRUpdate(float value, LimitType type);    // ratio
  void engLimitEGTUpdate(float value, LimitType type);    // deg C
  void engLimitTRQUpdate(float value, LimitType type);    // ft-lb
  void engLimitITTUpdate(float value, LimitType type);    // deg C
  void engLimitCHTUpdate(float value, LimitType type);    // deg C

  void engLimitOilPUpdate(float value, LimitType type);   // PSI
  void engLimitOilTUpdate(float value, LimitType type);   // deg C
  void engLimitFuelPUpdate(float value, LimitType type);  // PSI

private:
  CPDSettings* cpdSettings;
  SwitchBoard* sb;
  AircraftEngineType engType;
  int numEngines;
  int numFuelTanks;
};

typedef EngineController ENGC;

#endif  // ENGINECONTROLLER_H

