/* 
 * File:   mapoverlay.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 15, 2015, 2:36 PM
 */

#include "mapoverlay.h"

#include <algorithm>
#include <math.h>
#include <QPainter>

#include "qt-google-maps/mapsettings.h"
#include "core/hddsettings.h"
#include "core/mapconsts.h"


#define PI 3.1415926535 

using namespace std;

MapOverlay::MapOverlay(HDDSettings* _hddSettings, MapSettings* _mapSettings, QWidget* _parent)
: QWidget(_parent),
  hddSettings(_hddSettings),
  mapSettings(_mapSettings),
  heading(0.0)
{
   initRangeTicks();
   
   setAttribute(Qt::WA_NoSystemBackground);
   setAttribute(Qt::WA_TransparentForMouseEvents);
   
   aircraftIcon = QImage(":/ac/icons/airplane.png");
   setMinimumSize(QSize(DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT));
}

//MapOverlay::MapOverlay(const MapOverlay& orig)
//{
//}

MapOverlay::~MapOverlay()
{
}


QSize MapOverlay::sizeHint() const
{
   return QSize(DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT);
}


void MapOverlay::resize(int w, int h)
{
   resize(QSize(w, h));
}


/*
 * This widget should always be a square.
 */
void MapOverlay::resize(const QSize& size)
{
   QWidget::resize(size);
   
   // Update the range ticks
   initRangeTicks(size.width());
}


void MapOverlay::setZoom(int level)
{
   // TODO: set circle radius in proportion to zoom level
}


void MapOverlay::panToLocation(float lat, float lon)
{
   // TODO: change circle position or something
}


/*
 * Returns a line object originating at the center of the overlay window, with
 * the following properties:
 *    deg  = the angle in degrees
 *    from = inside radius to draw in pixels
 *    to   = outside outside to draw in pixels
 *    cx   = center point x in pixels; default = 0
 *    cy   = center point y in pixels; default = 0
 * 
 * Basically, the line is centered at (cx, cy), and goes out at an angle deg, 
 * and is only drawn between the radius' from and to.
 */
QLineF MapOverlay::getLine(double deg, int from, int to, int cx, int cy)
{
   // Convert angle to radians, also add 90 to orient it correctly
   // (0 deg is up, not right, as drawn by default)
   double drawAng = deg - 90;
   if (drawAng < 0) {
      drawAng = deg + 270;
   }
   double rad = (drawAng) * PI/180;
   
   // First point is on the radius circle
   double x1 = cx + (to * cos(rad));
   double y1 = cy + (to * sin(rad));
   
   // Second point is the aircraft position (0)
   int offset = 20; // allow a gap around the aircraft itself
   double x2 = cx + (from * cos(rad));
   double y2 = cy + (from * sin(rad));
   
   return QLineF(x1, y1, x2, y2);
}


void MapOverlay::initRangeTicks(int diameter)
{
   // Ensure the vector is empty
   if (rangeCircleTicks.size() > 0) {
      rangeCircleTicks.clear();
   }
   
   int mapRad = diameter / 2;
   int cx = mapRad;
   int cy = mapRad;
   
   //// NORTH UP
   // Generate a line/tick for each degree on the compass
   for (int i = 0; i < 360; i++) {
      int len = 5;
      if      (i % 10 == 0) { len = 12; }
      else if (i %  5 == 0) { len = 8;  }
      
      QLineF line = getLine(i, mapRad - len, mapRad);
      rangeCircleTicks.append(line);
   }
}


void MapOverlay::paintEvent(QPaintEvent*)
{
   QPainter p(this);
   int centerX = width()/2;
   int centerY = height()/2;
   
   // Draw the center airplane icon, using the current orientation setting
   if (northUp()) {
      p.translate(centerX, centerY);
      p.rotate(heading);
      int x = 0 - (aircraftIcon.width()/2);
      int y = 0 - (aircraftIcon.height()/2);
      p.drawImage(QPoint(x,y), aircraftIcon);
      p.resetTransform();
   }
   else {
      // Draw the plane facing up
      int x = centerX - (aircraftIcon.width()/2);
      int y = centerY - (aircraftIcon.height()/2);
      p.drawImage(QPoint(x,y), aircraftIcon);
   }
   
   // Draw the range circle
   drawRangeCircle(p);
}


void MapOverlay::drawRangeCircle(QPainter& p)
{
   QPen origPen = p.pen();
   int cx = width()/2;  // center x
   int cy = height()/2; // center y
   
   QPen pen(Qt::SolidLine);
   pen.setColor(Qt::yellow);
   pen.setWidth(2);
   p.setPen(pen);
   
   // Draw the range circle
   // circles are 0 -> (5760=16*360), in 1/16th deg (integer) increments
   p.drawArc(rect(), 0, 5760);
   
   // Draw the tick marks
   p.translate(cx, cy);
   if (northUp()) {
      p.rotate(heading);
   }
   pen.setWidth(1);
   p.setPen(pen);
   p.drawLines(rangeCircleTicks);
   
   // Draw the heading, which will always be at 0 degrees, since we already
   // rotated the whole painter, if was necessary
   QLineF headingLine = getLine(0, 20, cx);
   p.drawLine(headingLine);
   
   // Reset for next drawing
   p.setPen(origPen);
   p.resetTransform();
}
