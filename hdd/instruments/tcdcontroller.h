/* 
 * File:   tcdcontroller.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 11, 2015, 12:02 PM
 * 
 * Turn Coordinator
 */

#ifndef TCDCONTROLLER_H
#define TCDCONTROLLER_H

#include <QObject>

#include "WidgetTC.h"

class TCDController : public QObject {
   Q_OBJECT;

public:
   TCDController(QObject* _parent = 0);
   TCDController(const TCDController& orig);
   virtual ~TCDController();

public slots:
   void update()              { tcdWidget->update(); }
   void setTurnRate(float tr) { tcdWidget->setTurnRate(tr); }
   void setSlipSkid(float ss) { tcdWidget->setSlipSkid(ss); }

private:
   WidgetTC* tcdWidget;
};

#endif	/* TCDCONTROLLER_H */

