/* 
 * File:   mapcontroller.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 11, 2015, 12:33 PM
 */

#include "mapcontroller.h"

#include <QDebug>

#include "mapsettings.h"
#include "mapwidget.h"

MapController::MapController(QObject* _parent)
: QObject(_parent)
{
   orientation = TRACK_UP;
   settings = new MapSettings(QString(), this);
   mapWidget = new MapWidget(settings);
   
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

void MapController::setOrientation(MapOrientation mo)
{
//   settings->setOrientation(mo);
   mapWidget->setOrientation(mo);
}
