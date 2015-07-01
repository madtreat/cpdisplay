/* 
 * File:   enginewidget.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 15, 2015, 2:37 PM
 */

#ifndef ENGINEWIDGET_
#define ENGINEWIDGET_

#include <QWidget>
#include <QProgressBar>
#include <QLabel>

class HDDSettings;

typedef QList<QProgressBar*> Throttles;

struct ThrottleWidget {
   QLabel* label;
   QProgressBar* throttle;
};


class EngineWidget : public QWidget {
   Q_OBJECT;

public:
   EngineWidget(HDDSettings* _hddSettings, int _numThrottles=2, QWidget* _parent = 0);
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
   int numThrottles;

   Throttles throttles;
   
   ThrottleWidget* createThrottle(int engNum, int max, int value);
   void setupEngineControls();
};

#endif	/* ENGINEWIDGET_ */

