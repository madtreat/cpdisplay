/* 
 * File:   vsicontroller.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 11, 2015, 12:02 PM
 * 
 * Vertical Speed Indicator
 */

#ifndef VSICONTROLLER_H
#define VSICONTROLLER_H

#include <QObject>

#include "WidgetVSI.h"

class VSIController : public QObject {
   Q_OBJECT;

public:
   VSIController(QObject* _parent = 0);
   VSIController(const VSIController& orig);
   virtual ~VSIController();

public slots:
   void update()                 { vsiWidget->update(); }
   void setClimbRate(float cr)   { vsiWidget->setClimbRate(cr); }

private:
   WidgetVSI* vsiWidget;
};

#endif	/* VSICONTROLLER_H */

