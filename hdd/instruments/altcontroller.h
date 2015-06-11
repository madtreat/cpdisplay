/* 
 * File:   altcontroller.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 11, 2015, 11:35 AM
 * 
 * Barometric Altimeter
 */

#ifndef ALTCONTROLLER_H
#define ALTCONTROLLER_H

#include <QObject>

#include "WidgetALT.h"

class ALTController : public QObject {
   Q_OBJECT;

public:
   ALTController(QObject* _parent = 0);
   ALTController(const ALTController& orig);
   virtual ~ALTController();

public slots:
   void update()                 { altWidget->update();           }
   void setAltitude(float alt)   { altWidget->setAltitude(alt);   }
   void setPressure(float p)     { altWidget->setPressure(p);     }

private:
   WidgetALT* altWidget;
};

#endif	/* ALTCONTROLLER_H */

