/* 
 * File:   mapcontroller.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 11, 2015, 12:33 PM
 */

#include "mapcontroller.h"

#include <QDebug>

#include "core/aircraft.h"
#include "core/cpdsettings.h"
#include "qt-google-maps/mapsettings.h"


MapController::MapController(CPDSettings* _cpdSettings, ACMap* _acMap, QObject* _parent)
: QObject(_parent),
  cpdSettings(_cpdSettings),
  acMap(_acMap)
{
   orientation = TRACK_UP;
   settings = new MapSettings("maps.ini", this);
   
   qDebug() << "Can enable maps?" << settings->canEnableMaps();
}

//MapController::MapController(const MapController& orig)
//{
//}

MapController::~MapController()
{
}

void MapController::setZoom(int level)
{
   if (level >= ZOOM_MAX) {
      emit zoomMaxReached(true);
      return;
   }
   else if (level <= ZOOM_MIN) {
      emit zoomMinReached(true);
      return;
   }
   settings->setZoom(level);
   emit updateZoom(level);
   emit zoomEither(true);
}

void MapController::increaseZoom()
{
   int newLevel = settings->zoom() + 1;
   setZoom(newLevel);
}

void MapController::decreaseZoom()
{
   int newLevel = settings->zoom() - 1;
   setZoom(newLevel);
}

void MapController::setOrientation(MapOrientation mo)
{
   settings->setMapOrientation(mo);
}
