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
   
signals:

private:
   HDDSettings* hddSettings;
   TrafficWidget* trafficWidget;
};

#endif	/* TRAFFICCONTROLLER_H */

