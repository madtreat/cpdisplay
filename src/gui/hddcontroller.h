/* 
 * File:   hddcontroller.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 15, 2015, 6:40 PM
 */

#ifndef HDDCONTROLLER_H
#define HDDCONTROLLER_H

#include <QObject>

#define PI 3.14159265

class HDDSettings;
class SwitchBoard;
class HDDWindow;

class HDDController : public QObject {
   Q_OBJECT;

public:
   HDDController(HDDSettings* _settings, QObject* _parent = 0);
   HDDController(const HDDController& orig) = delete;
   virtual ~HDDController();

   float deg2rad(float deg) { return deg*PI/180; }
   float calculateTurnRate(float q, float r, float pitch, float roll);

public slots:
   void updateAngVel(float q, float p, float r);
   void updatePitch(float p);
   void updateRoll(float r);
   void tryCalculateTurnRate();

signals:
   void turnRateUpdate(float tr);

private:
   HDDSettings*   settings;
   SwitchBoard*   sb;
   HDDWindow*     window;

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

