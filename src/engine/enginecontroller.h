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
  EngineController(CPDSettings* _cpdSettings, SwitchBoard* _sb, AircraftType _engType, int _numEngines, int _numFuelTanks, QObject* _parent = 0);
  EngineController(const EngineController& orig) = delete;
  virtual ~EngineController();

  AircraftType getEngineType() const { return engType; }

public slots:
  void updateACType(int acType);
  void updateNumEngines(int numEng);
  void updateThrottleCommand(float throttle, int engNum);
  void updateThrottleActual(float throttle, int engNum);

signals:
  void engineTypeUpdate(AircraftType type);
  void numEnginesUpdate(int numEngines);
  void numTanksUpdate(int numTanks);

  void throttleUpdate(int value, int engNum);
  void engPowerUpdate(float value, int engNum);
  void engThrustUpdate(float value, int engNum);
  void engTorqueUpdate(float value, int engNum);
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
  AircraftType engType;
  int numEngines;
  int numFuelTanks;
};

typedef EngineController ENGC;

#endif  // ENGINECONTROLLER_H

