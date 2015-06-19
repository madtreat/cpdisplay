/* 
 * File:   adicontroller.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 11, 2015, 10:58 AM
 * 
 * Attitude Director Indicator
 */

#ifndef ADICONTROLLER_H
#define ADICONTROLLER_H

#include <QObject>

#include "WidgetADI.h"

class ADIController : public QObject {
   Q_OBJECT;
   
public:
   ADIController(QObject* _parent=0);
   ADIController(const ADIController& orig);
   virtual ~ADIController();

   WidgetADI* getWidget() const { return adiWidget; }
   
public slots:
   void update()           { adiWidget->update();  }
   void setRoll(float r)   { adiWidget->setRoll(r);   update(); }
   void setPitch(float p)  { adiWidget->setPitch(p);  update(); }
   
private:
   WidgetADI* adiWidget;
};

#endif	/* ADICONTROLLER_H */

