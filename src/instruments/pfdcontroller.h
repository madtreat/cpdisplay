/*
 * File:   pfdcontroller.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 11, 2015, 10:58 AM
 * 
 * This class controls the PFD, connecting the displayed widget to the data
 * source (X-Plane).
 */

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
   void update()                 { pfdWidget->update();        }
   void setRoll(float roll)      { pfdWidget->setRoll(roll);   }
   void setPitch(float pitch)    { pfdWidget->setPitch(pitch); }
   void setSlipSkid(float ss)    { pfdWidget->setSlipSkid(ss); }
   void setDevH(float devH)      { pfdWidget->setDevH(devH);   }
   void setDevV(float devV)      { pfdWidget->setDevV(devV);   }
   void setAltitude(float alt)   { pfdWidget->setAltitude(alt);}
   void setPressure(float p)     { pfdWidget->setPressure(p);  }
   void setAirspeed(float as)    { pfdWidget->setAirspeed(as); }
   void setMachNo(float m)       { pfdWidget->setMachNo(m);    }
   void setHeading(float h)      { pfdWidget->setHeading(h);   }
   void setTurnRate(float tr)    { pfdWidget->setTurnRate(tr); }
   void setClimbRate(float cr)   { pfdWidget->setClimbRate(cr);}
   void setFlightPathMarker(float aoa, float sideSlip) { pfdWidget->setFlightPathMarker(aoa, sideSlip); }

private:
   WidgetPFD* pfdWidget;
};
