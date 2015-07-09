/* 
 * File:   hddcontroller.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 15, 2015, 6:40 PM
 */

#ifndef HDDCONTROLLER_H
#define HDDCONTROLLER_H

#include <QObject>

#include "core/aircraft.h"
#include "map/mapcontroller.h"

#define PI 3.14159265

class HDDSettings;
class SwitchBoard;
class HDDWindow;

//class MapController;
class MapView;
class MapOverlay;

class ADIController;
class ALTController;
class ASIController;
class HSIController;
class PFDController;
class TCDController;
class VSIController;

class CommsController;
class EngineController;
class TrafficController;

class HDDController : public QObject {
   Q_OBJECT;

public:
   HDDController(HDDSettings* _settings, QObject* _parent = 0);
   HDDController(const HDDController& orig) = delete;
   virtual ~HDDController();

   ACMap* getACMap() const { return acMap; }
   int    getNumEngines() const { return numEngines; }
   
   MapController* getMapC() const { return mapC; }
   ADIController* getADIC() const { return adiC; }
   ALTController* getALTC() const { return altC; }
   ASIController* getASIC() const { return asiC; }
   HSIController* getHSIC() const { return hsiC; }
   PFDController* getPFDC() const { return pfdC; }
   TCDController* getTCDC() const { return tcdC; }
   VSIController* getVSIC() const { return vsiC; }
   
   CommsController*   getComC() const { return comC; }
   EngineController*  getEngC() const { return engC; }
   TrafficController* getTfcC() const { return tfcC; }
   
   MapView*    getMapView() const { return mapC->getMapView(); }
   MapOverlay* getOverlay() const { return mapC->getOverlay(); }
   
   float calculateTurnRate(float q, float r, float pitch, float roll);
   
   Aircraft* createAircraft(int id, float lat, float lon, float alt);

public slots:
   void updateAngVel(float q, float p, float r);
   void updatePitch(float p);
   void updateRoll(float r);
   void tryCalculateTurnRate();
   
   void updateACLat(float lat, int ac);
   void updateACLon(float lon, int ac);
   void updateACAlt(float alt, int ac);
   
   void acUpdated(int id);

signals:
   void turnRateUpdate(float tr);

private:
   HDDSettings*   settings;
   SwitchBoard*   sb;
//   HDDWindow*     window;
   ACMap*         acMap;
   int            numEngines;
   
   // Widget controllers
   MapController* mapC;
   ADIController* adiC;
   ALTController* altC;
   ASIController* asiC;
   HSIController* hsiC;
   PFDController* pfdC;
   TCDController* tcdC;
   VSIController* vsiC;
   
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

#endif	/* HDDCONTROLLER_H */

