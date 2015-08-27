/* 
 * File:   trafficcontroller.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 23, 2015, 12:33 PM
 */

#ifndef TRAFFICCONTROLLER_H
#define TRAFFICCONTROLLER_H

#include <QObject>

#include "core/aircraft.h"


class CPDSettings;

class TrafficController : public QObject {
   Q_OBJECT;

public:
   TrafficController(CPDSettings* _cpdSettings, ACMap* _acMap, QObject* _parent = 0);
   TrafficController(const TrafficController& orig) = delete;
   virtual ~TrafficController();

public slots:
   void acUpdated(int id);
   void updateCurrentAC(int id);
   
signals:
   void displayedACUpdated(int id);
   void acRngBerUpdated(int id);

private:
   CPDSettings*   cpdSettings;
   ACMap*         acMap;
   
   int currentID; // currently displayed AC ID
};

typedef TrafficController TFCC;

#endif	/* TRAFFICCONTROLLER_H */

