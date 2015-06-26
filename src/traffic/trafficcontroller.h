/* 
 * File:   trafficcontroller.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 23, 2015, 12:33 PM
 */

#ifndef TRAFFICCONTROLLER_H
#define TRAFFICCONTROLLER_H

#include <QObject>

#include "trafficwidget.h"
#include "core/aircraft.h"


class HDDSettings;
class TrafficWidget;

class TrafficController : public QObject {
   Q_OBJECT;

public:
   TrafficController(HDDSettings* _hddSettings, QObject* _parent = 0);
   TrafficController(const TrafficController& orig) = delete;
   virtual ~TrafficController();
   
   TrafficWidget*  getWidget()  const { return trafficWidget; }

public slots:
   void setDisplayedAC(Aircraft* ac);
   
signals:

private:
   HDDSettings*   hddSettings;
   TrafficWidget* trafficWidget;
   ACMap          acMap;
   
   int currentID; // currently displayed AC ID
};

#endif	/* TRAFFICCONTROLLER_H */

