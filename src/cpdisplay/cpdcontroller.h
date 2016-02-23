/* 
 * File:   cpdcontroller.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 15, 2015, 6:40 PM
 */

#ifndef CPDCONTROLLER_H
#define CPDCONTROLLER_H

#include <QObject>

#include "core/aircraft.h"
#include "map/mapcontroller.h"

#define PI 3.14159265

class CPDSettings;
class SwitchBoard;

class MapController;

class ADIController;
class ALTController;
class ASIController;
class HSIController;
class PFDController;
class TCDController;
class VSIController;

class GearController;
class CommsController;
class EngineController;
class TrafficController;


class CPDController : public QObject {
  Q_OBJECT;

public:
  CPDController(CPDSettings* _settings, int _slaveID = -1, QObject* _parent = 0);
  CPDController(const CPDController& orig) = delete;
  virtual ~CPDController();

  ACMap* getACMap() const { return acMap; }
  int    getNumEngines() const { return numEngines; }

  SwitchBoard* getSwitchBoard() const { return sb; }

  MapController* getMapC() const { return mapC; }
  ADIController* getADIC() const { return adiC; }
  ALTController* getALTC() const { return altC; }
  ASIController* getASIC() const { return asiC; }
  HSIController* getHSIC() const { return hsiC; }
  PFDController* getPFDC() const { return pfdC; }
  TCDController* getTCDC() const { return tcdC; }
  VSIController* getVSIC() const { return vsiC; }

  GearController*    getGearC() const { return gearC; }
  CommsController*   getComC()  const { return comC;  }
  EngineController*  getEngC()  const { return engC;  }
  TrafficController* getTfcC()  const { return tfcC;  }

  float calculateTurnRate(float q, float r, float pitch, float roll);

  Aircraft* createAircraft(int id, float lat, float lon, float alt);

public slots:
  void updateAngVel(float q, float p, float r);
  void updatePitch(float p);
  void updateRoll(float r);
  void tryCalculateTurnRate();

  // Multiplayer aircraft updates
  void updateACLat(float lat, int ac);
  void updateACLon(float lon, int ac);
  void updateACAlt(float alt, int ac);
  void updateACHdg(float hdg, int ac);
  void updateACSpdX(float vx, int ac);
  void updateACSpdY(float vy, int ac);
  void updateACSpdZ(float vz, int ac);

  void printSomething(float value); // for general testing

signals:
  void turnRateUpdate(float tr);
  void updateGearRetractable(float retractable);

private:
  CPDSettings*   settings;
  int            slaveID;
  SwitchBoard*   sb;
  ACMap*         acMap;
  int            numEngines;
  int            numFuelTanks;
  int            numGears;
   
  // Widget controllers
  MapController* mapC;
  ADIController* adiC;
  ALTController* altC;
  ASIController* asiC;
  HSIController* hsiC;
  PFDController* pfdC;
  TCDController* tcdC;
  VSIController* vsiC;

  GearController*    gearC;
  CommsController*   comC;
  EngineController*  engC;
  TrafficController* tfcC;

  // For calculating turn rate
  float angVelQ;
  float angVelP;
  float angVelR;
  float pitch;
  float roll;

  bool  angVelUpdatedFlag;
  bool  pitchUpdatedFlag;
  bool  rollUpdatedFlag;

  void connectSignals();
};

typedef CPDController CPDC;

#endif	/* CPDCONTROLLER_H */

