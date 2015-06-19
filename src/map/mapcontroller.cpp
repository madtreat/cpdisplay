/* 
 * File:   mapcontroller.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 11, 2015, 12:33 PM
 */

#include "mapcontroller.h"

#include <QDebug>

#include "hddsettings.h"
#include "mapsettings.h"
#include "mapwidget.h"

MapController::MapController(HDDSettings* _hddSettings, QObject* _parent)
: QObject(_parent),
  hddSettings(_hddSettings)
{
   orientation = TRACK_UP;
   settings = new MapSettings(QString(), this);
   mapWidget = new MapWidget(hddSettings, settings);
   
   qDebug() << "Can enable maps?" << settings->canEnableMaps();

//   connect(settings, SIGNAL(zoomChanged(int)),  mapWidget, SLOT(updateZoomLevel(int)));
//   connect(this, SIGNAL(incrementZoom()),       mapWidget, SLOT(incrementZoom()));
//   connect(this, SIGNAL(decrementZoom()),       mapWidget, SLOT(decrementZoom()));
   
//   connect(this, SIGNAL(updateOrientation(MapOrientation)), mapWidget, SLOT(updateOrientation(MapOrientation)));
   
//   emit updateOrientation(orientation);
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
   mapWidget->setZoom(level);
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

void MapController::panToLocation(float lat, float lon, int aircraft)
{
   mapWidget->panToLocation(lat, lon);
}

void MapController::setOrientation(MapOrientation mo)
{
   hddSettings->setMapOrientation(mo);
   mapWidget->setOrientation(mo);
}

void MapController::setHeading(float heading)
{
   mapWidget->setHeading(heading);
}
