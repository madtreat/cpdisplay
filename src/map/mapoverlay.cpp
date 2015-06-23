/* 
 * File:   mapoverlay.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 15, 2015, 2:36 PM
 */

#include "mapoverlay.h"

#include <QPainter>

#include "qt-google-maps/mapsettings.h"
#include "core/hddsettings.h"

MapOverlay::MapOverlay(HDDSettings* _hddSettings, MapSettings* _mapSettings, QWidget* _parent)
: QWidget(_parent),
  hddSettings(_hddSettings),
  mapSettings(_mapSettings),
  heading(0)
{
   setAttribute(Qt::WA_NoSystemBackground);
   setAttribute(Qt::WA_TransparentForMouseEvents);
   
   aircraftIcon = QImage(":/ac/icons/airplane.png");
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
   int centerX = width()/2;
   int centerY = height()/2;
   
   // Draw the center airplane icon, using the current orientation setting
   if (hddSettings->mapOrientation() == NORTH_UP) {
      p.translate(centerX, centerY);
      p.rotate(heading);
      int x = 0/*centerX*/ - (aircraftIcon.width()/2);
      int y = 0/*centerY*/ - (aircraftIcon.height()/2);
      p.drawImage(QPoint(x,y), aircraftIcon);
      p.resetTransform();
   }
   else {
      // Draw the plane facing up
      int x = centerX - (aircraftIcon.width()/2);
      int y = centerY - (aircraftIcon.height()/2);
      p.drawImage(QPoint(x,y), aircraftIcon);
   }
}
