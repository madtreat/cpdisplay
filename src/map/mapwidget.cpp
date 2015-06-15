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

MapWidget::MapWidget(MapSettings* _settings, QWidget* _parent)
: QWidget(_parent),
  settings(_settings)
{
   QGridLayout* layout = new QGridLayout(this);
   view = new MapView(settings);
   layout->addWidget(view);
   
   overlay = new MapOverlay(settings, view);
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

//void MapWidget::incrementZoom()
//{
//   if (settings->zoom() < ZOOM_MAX) {
//      currentZoom++;
//      updateZoomLevel(currentZoom);
//   }
//}
//
//void MapWidget::decrementZoom()
//{
//   if (settings->zoom() > ZOOM_MIN) {
//      currentZoom--;
//      updateZoomLevel(currentZoom);
//   }
//}

void MapWidget::setOrientation(MapOrientation mo)
{
   // TODO: figure out how to change this
}
