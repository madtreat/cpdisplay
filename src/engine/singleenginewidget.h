/* 
 * File:   singleenginewidget.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on July 31, 2015, 12:54 PM
 */

#ifndef SINGLEENGINEWIDGET_H
#define SINGLEENGINEWIDGET_H

#include <QWidget>


class HDDSettings;
class EngineController;
class ThrottleWidget;
class EngineDial;


class SingleEngineWidget : public QWidget {
   Q_OBJECT;

public:
   SingleEngineWidget(HDDSettings* _hddSettings, EngineController* _engC, int _engNum, QWidget* _parent = 0);
   SingleEngineWidget(const SingleEngineWidget& orig) = delete;
   virtual ~SingleEngineWidget();

public slots:
   void setOilPressure(float pressure, int _engNum);
   void setOilTemp(float temp, int _engNum);
   void setEngRPM(float rpm, int _engNum);

private:
   HDDSettings*      hddSettings;
   EngineController* engC;
   int               engNum;
   
   ThrottleWidget*   throttle;
   EngineDial*       oilP; // pressure gauge
   EngineDial*       oilT; // temperature gauge
   EngineDial*       engRPM; // RPM gauge
};

#endif	/* SINGLEENGINEWIDGET_H */

