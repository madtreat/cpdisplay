/*
 * File:   pfdcontroller.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 11, 2015, 10:58 AM
 * 
 * This class controls the PFD, connecting the displayed widget to the data
 * source (X-Plane).
 */

#ifndef PFDCONTROLLER_H
#define PFDCONTROLLER_H

#include <QObject>
#include <QWidget>

#include "WidgetPFD.h"
//class WidgetPFD;

class PFDController : public QObject {
   Q_OBJECT;

public:
   PFDController(QObject* _parent=0);
   PFDController(const PFDController& orig);
   virtual ~PFDController();

   WidgetPFD* getWidget() const  { return pfdWidget; }

public slots:
   // Connect to WidgetPFD functions
   void update()                 { pfdWidget->update(); }
   void setRoll(float roll)      { pfdWidget->setRoll(roll);      update(); }
   void setPitch(float pitch)    { pfdWidget->setPitch(pitch);    update(); }
   void setSlipSkid(float ss)    { pfdWidget->setSlipSkid(ss);    update(); }
   void setDevH(float devH)      { pfdWidget->setDevH(devH);      update(); }
   void setDevV(float devV)      { pfdWidget->setDevV(devV);      update(); }
   void setAltitude(float alt)   { pfdWidget->setAltitude(alt);   update(); }
   void setPressure(float p)     { pfdWidget->setPressure(p);     update(); }
   void setAirspeed(float as)    { pfdWidget->setAirspeed(as);    update(); }
   void setMachNo(float m)       { pfdWidget->setMachNo(m);       update(); }
   void setHeading(float h)      { pfdWidget->setHeading(h);      update(); }
   void setTurnRate(float tr)    { pfdWidget->setTurnRate(tr);    update(); }
   void setClimbRate(float cr)   { pfdWidget->setClimbRate(cr);   update(); }
   void setFlightPathMarker(float aoa, float sideSlip) { pfdWidget->setFlightPathMarker(aoa, sideSlip); update(); }

private:
   WidgetPFD* pfdWidget;
};

typedef PFDController PFDC;

#endif
