/* 
 * File:   hsicontroller.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 11, 2015, 11:46 AM
 * 
 * Horizontal Situation Indicator
 */

#ifndef HSICONTROLLER_H
#define HSICONTROLLER_H

#include <QObject>

#include "WidgetHSI.h"

class HSIController : public QObject {
   Q_OBJECT;

public:
   HSIController(QObject* _parent = 0);
   HSIController(const HSIController& orig);
   virtual ~HSIController();

   WidgetHSI* getWidget() const { return hsiWidget; }
   
public slots:
   void update()              { hsiWidget->update(); }
   void setHeading(float h)   { hsiWidget->setHeading(h); update(); }

private:
   WidgetHSI* hsiWidget;
};

typedef HSIController HSIC;

#endif	/* HSICONTROLLER_H */

