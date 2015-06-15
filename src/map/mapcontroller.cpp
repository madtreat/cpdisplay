/* 
 * File:   mapcontroller.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 11, 2015, 12:33 PM
 */

#include "mapcontroller.h"
#include "mapsettings.h"
#include "mapwidget.h"

MapController::MapController(QObject* _parent)
: QObject(_parent)
{
   orientation = TRACK_UP;
   settings = new MapSettings(QString(), this);
   mapWidget = new MapWidget(settings);
   
   connect(this, SIGNAL(updateZoomLevel(int)),  mapWidget, SLOT(updateZoomLevel(int)));
   connect(this, SIGNAL(incrementZoom()),       mapWidget, SLOT(incrementZoom()));
   connect(this, SIGNAL(decrementZoom()),       mapWidget, SLOT(decrementZoom()));
   
   connect(this, SIGNAL(updateOrientation(MapOrientation)), mapWidget, SLOT(updateOrientation(MapOrientation)));
   
   emit updateOrientation(orientation);
}

//MapController::MapController(const MapController& orig)
//{
//}

MapController::~MapController()
{
}

