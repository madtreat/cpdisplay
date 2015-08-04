/* 
 * File:   asicontroller.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 11, 2015, 11:40 AM
 * 
 * Airspeed Indicator
 */

#ifndef ASICONTROLLER_H
#define ASICONTROLLER_H

#include <QObject>

#include "WidgetASI.h"

class ASIController : public QObject {
   Q_OBJECT;

public:
   ASIController(QObject* _parent = 0);
   ASIController(const ASIController& orig);
   virtual ~ASIController();

   WidgetASI* getWidget() const { return asiWidget; }
   
public slots:
   void update()              { asiWidget->update(); }
   void setAirspeed(float as) { asiWidget->setAirspeed(as); update(); }

private:
   WidgetASI* asiWidget;
};

typedef ASIController ASIC;

#endif	/* ASICONTROLLER_H */

