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


class CPDSettings;
class TrafficWidget;

class TrafficController : public QObject {
   Q_OBJECT;

public:
   TrafficController(CPDSettings* _cpdSettings, ACMap* _acMap, QObject* _parent = 0);
   TrafficController(const TrafficController& orig) = delete;
   virtual ~TrafficController();
   
   TrafficWidget*  getWidget()  const { return trafficWidget; }

public slots:
   void acUpdated(int id);
   void setDisplayedAC(Aircraft* ac);
   void updateCurrentAC(int id);
   
signals:
   void acRngBerUpdated(int id);

private:
   CPDSettings*   cpdSettings;
   TrafficWidget* trafficWidget;
   ACMap*         acMap;
   
   int currentID; // currently displayed AC ID
};

typedef TrafficController TFCC;

#endif	/* TRAFFICCONTROLLER_H */

