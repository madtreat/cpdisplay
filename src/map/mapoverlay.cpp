/* 
 * File:   mapoverlay.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 15, 2015, 2:36 PM
 */

#include "mapoverlay.h"

#include <QPainter>

#include "mapsettings.h"

MapOverlay::MapOverlay(MapSettings* _settings, QWidget* _parent)
: QWidget(_parent),
  settings(_settings)
{
   setAttribute(Qt::WA_NoSystemBackground);
   setAttribute(Qt::WA_TransparentForMouseEvents);
}

//MapOverlay::MapOverlay(const MapOverlay& orig)
//{
//}

MapOverlay::~MapOverlay()
{
}

void MapOverlay::setZoom(int level)
{
   // TODO: set circle radius in proportion to zoom level
}

void MapOverlay::panToLocation(float lat, float lon)
{
   // TODO: change circle position or something
}

void MapOverlay::paintEvent(QPaintEvent*)
{
   QPainter p(this);
//   p.fillRect(rect(), QColor(80, 80, 255, 128));
}