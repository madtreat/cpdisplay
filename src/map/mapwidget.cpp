/* 
 * File:   mapwidget.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 15, 2015, 2:36 PM
 */

#include "mapwidget.h"

#include <QGridLayout>

#include "mapsettings.h"
#include "mapview.h"
#include "mapoverlay.h"
#include "hddsettings.h"

MapWidget::MapWidget(HDDSettings* _hddSettings, MapSettings* _mapSettings, QWidget* _parent)
: QWidget(_parent),
  hddSettings(_hddSettings),
  mapSettings(_mapSettings)
{
   QGridLayout* layout = new QGridLayout(this);
   view = new MapView(hddSettings, mapSettings);
   layout->addWidget(view);
   
   overlay = new MapOverlay(hddSettings, mapSettings, view);
   overlay->setGeometry(view->geometry());
}

//MapWidget::MapWidget(const MapWidget& orig)
//{
//}

MapWidget::~MapWidget()
{
   delete view;
   delete overlay;
}

void MapWidget::setZoom(int level)
{
   view->setZoom(level);
   overlay->setZoom(level);
}

void MapWidget::panToLocation(float lat, float lon)
{
   view->panToLocation(lat, lon);
   overlay->panToLocation(lat, lon);
}

void MapWidget::setOrientation(MapOrientation mo)
{
   // The MapController::setOrientation() function directly updates the settings,
   // which the overlay uses.  But the MapView class still needs to change the
   // way it follows the aircraft.
   
}

void MapWidget::setHeading(float heading)
{
   view->setHeading(heading);
   overlay->setHeading(heading);
}
