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

TrafficController::TrafficController(HDDSettings* _hddSettings, ACMap* _acMap, QObject* _parent)
: QObject(_parent),
  hddSettings(_hddSettings),
  acMap(_acMap)
{
   trafficWidget = new TrafficWidget(hddSettings, acMap);
   connect(trafficWidget, &TrafficWidget::displayedACChanged, this, &TrafficController::updateCurrentAC);
   currentID = 1;
}

//TrafficController::TrafficController(const TrafficController& orig)
//{
//}

TrafficController::~TrafficController()
{
}

/*
 * This slot is called when any given aircraft (identified by id) has had its
 * lat, lon and altitude updated.
 * 
 * Update the map so it can draw the AC.
 * Update the TFC controller so it can update the displayed values if this AC
 * is currently selected.
 */
void TrafficController::acUpdated(int id)
{
   
}

void TrafficController::setDisplayedAC(Aircraft* ac)
{
   int id = ac->getID();
   if (!acMap->contains(id)) {
      acMap->insert(id, ac);
   }
   // TODO: update the tab widget or whatever
   if (id == currentID) {
      trafficWidget->displayAC(ac);
   }
}

void TrafficController::updateCurrentAC(int id)
{
   currentID = id;
}