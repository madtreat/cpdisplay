/* 
 * File:   trafficcontroller.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 23, 2015, 12:33 PM
 */

#include "trafficcontroller.h"

#include <QDebug>

#include "core/hddsettings.h"
#include "trafficwidget.h"

TrafficController::TrafficController(HDDSettings* _hddSettings, QObject* _parent)
: QObject(_parent),
  hddSettings(_hddSettings)
{
   trafficWidget = new TrafficWidget(hddSettings);
   currentID = 1;
}

//TrafficController::TrafficController(const TrafficController& orig)
//{
//}

TrafficController::~TrafficController()
{
}

void TrafficController::setDisplayedAC(Aircraft* ac)
{
   int id = ac->getID();
   if (!acMap.contains(id)) {
      acMap[id] = ac;
   }
   // TODO: update the tab widget or whatever
   if (id == currentID) {
      trafficWidget->displayAC(ac);
   }
}
