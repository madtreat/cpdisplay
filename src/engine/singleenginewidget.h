/* 
 * File:   singleenginewidget.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on July 31, 2015, 12:54 PM
 */

#ifndef SINGLEENGINEWIDGET_H
#define SINGLEENGINEWIDGET_H

#include <QFrame>
#include <QString>

#include "core/aircraft.h"


class QLabel;
class QHBoxLayout;
class QFormLayout;
class QWidget;

class CPDSettings;
class EngineController;
class ThrottleWidget;
class EngineDial;


class SingleEngineWidget : public QFrame {
   Q_OBJECT;

public:
   SingleEngineWidget(CPDSettings* _cpdSettings, EngineController* _engC, int _engNum, QFrame* _parent = 0);
   SingleEngineWidget(const SingleEngineWidget& orig) = delete;
   virtual ~SingleEngineWidget();

public slots:
   void setVisibleWidgets(AircraftEngineType type);
   
   void setEngRPM(float value, int _engNum);
   void setPropRPM(float value, int _engNum);
   void setPropPitch(float value, int _engNum);
   void setPropwash(float value, int _engNum);
   void setN1(float value, int _engNum);
   void setN2(float value, int _engNum);
   void setMP(float value, int _engNum);
   void setEPR(float value, int _engNum);
   void setFF(float value, int _engNum);
   void setITT(float value, int _engNum);
   void setEGT(float value, int _engNum);
   void setCHT(float value, int _engNum);
   void setOilPressure(float value, int _engNum);
   void setOilTemp(float value, int _engNum);

private:
   CPDSettings*      cpdSettings;
   EngineController* engC;
   int               engNum;
   
   QHBoxLayout*      upper; // upper layout (throttle + form layouts)
   QWidget*          propForm; // for quick layout swapping
   QWidget*          jetForm;  // for quick layout swapping
   
   // Graphical widgets
   ThrottleWidget*   throttle;
   EngineDial*       oilP; // pressure gauge
   EngineDial*       oilT; // temperature gauge
   EngineDial*       engRPM; // engine RPM gauge
   EngineDial*       propRPM; // prop RPM gauge
   EngineDial*       eprDial;
   EngineDial*       egtDial;
   
   // Textual display widgets
   QLabel*           propPitch;
   QLabel*           propWash;
   QLabel*           n1;
   QLabel*           n2;
   QLabel*           mp; // manifold pressure
   QLabel*           epr; // engine pressure ratio
   QLabel*           fuelFlow;
   QLabel*           itt; // ???
   QLabel*           egt; // exhaust gas temp
   QLabel*           cht; // ???
   
   // Text for units on widgets
   QString propPitchUnits;
   QString propWashUnits;
   QString n1Units;
   QString n2Units;
   QString mpUnits;
   QString eprUnits;
   QString ffUnits;
   QString ittUnits;
   QString egtUnits;
   QString chtUnits;
};

#endif	/* SINGLEENGINEWIDGET_H */

