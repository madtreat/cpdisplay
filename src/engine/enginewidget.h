/* 
 * File:   enginewidget.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 15, 2015, 2:37 PM
 */

#ifndef ENGINEWIDGET_
#define ENGINEWIDGET_

#include <QFrame>
#include "throttlewidget.h"

class HDDSettings;

class EngineWidget : public QFrame {
   Q_OBJECT;

public:
   EngineWidget(HDDSettings* _hddSettings, int _numEngines=2, QFrame* _parent = 0);
   EngineWidget(const EngineWidget& orig) = delete;
   virtual ~EngineWidget();

public slots:
   void updateThrottleCommand(float throttle, int engNum);
   void updateThrottleActual(float throttle, int engNum);
   void updateEngPower(float power, int engNum);
   void updateEngThrust(float thrust, int engNum);
   void updateEngTorque(float torque, int engNum);
   void updateEngRPM(float rpm, int engNum);

private:
   HDDSettings* hddSettings;
   int numEngines;

   ThrottleList throttles;
   
   void setupEngineControls();
};

#endif	/* ENGINEWIDGET_ */

