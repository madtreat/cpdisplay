/* 
 * File:   trafficcontroller.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 23, 2015, 12:33 PM
 */

#include "trafficcontroller.h"

#include <QDebug>

#include "utils/geodist.h"
#include "core/cpdsettings.h"


TrafficController::TrafficController(CPDSettings* _cpdSettings, ACMap* _acMap, QObject* _parent)
: QObject(_parent),
  cpdSettings(_cpdSettings),
  acMap(_acMap)
{
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
 * Update the aircraft's range and bearing from this cockpit's position.
 * If this is the currently displayed aircraft, update the values in the widget.
 */
void TrafficController::acUpdated(int id)
{
   Aircraft* ac0 = acMap->value(0);  // This cockpit/AC
   Aircraft* ac  = acMap->value(id); // The updated AC
   double rng = geo::distance(ac0->getLat(), ac0->getLon(), ac->getLat(), ac->getLon());
   double ber = geo::bearing(ac0->getLat(), ac0->getLon(), ac->getLat(), ac->getLon());
   ac->setRngBer(rng, ber);
   
   if (id == currentID) {
      emit displayedACUpdated(id);
   }
}

void TrafficController::updateCurrentAC(int id)
{
   currentID = id;
}
