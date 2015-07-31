/* 
 * File:   enginedial.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on July 31, 2015, 1:26 PM
 */

#include "enginedial.h"

#include <QPainter>
#include <QPolygon>


EngineDial::EngineDial(EngineDialType _type, QWidget* _parent)
: QDial(_parent),
  type(_type)
{
   if (type == OIL_TEMP) {
      typeText = "Oil T";
      valueTextBase = "_X_ deg C";
   }
   else if (type == OIL_PRESSURE) {
      typeText = "Oil P";
      valueTextBase = "_X_ psi";
   }
   else if (type == ENG_RPM) {
      typeText = "ENG RPM";
      valueTextBase = "_X_";
   }
   
   tickIncrement = 0.0;
   tickDegIncrement = 30;
   tickDegMin = 0;
   tickDegMax = 210;
   
   circleBuffer = 20;
}

//EngineDial::EngineDial(const EngineDial& orig)
//{
//}

EngineDial::~EngineDial()
{
}

void EngineDial::paintEvent(QPaintEvent*)
{
   QPainter p(this);
   if (tickIncrement == 0.0) {
      int max = maximum();
      int min = minimum();
      int range = max - min;
      double ratio = tickDegIncrement / (tickDegMax - tickDegMin);
      
      tickIncrement = range * ratio;
   }
   
   int extent = (width() > height()) ? height() : width();
   extent -= circleBuffer;
   
   p.translate((width()-extent)/2, (height()-extent)/2);
   p.setPen(QColor(105, 105, 105));
   p.setBrush(QColor(34, 34, 34));
   
   // Draw the background circle
   // Note the 15 deg offset
   p.drawArc(0, 0, extent, extent, 15*16, (tickDegMax-tickDegMin)*16);
   
   // Draw dial identification and value text
   p.setPen(QColor(0, 128, 0, 255));
   QString valueText = valueTextBase.replace("_X_", QString::number(value()));
   p.drawText(extent/2-4, extent/2+18, typeText);
   p.drawText(extent/2-18, extent/2+32, valueText);
   
   // Draw tickmarks
   p.translate(extent/2, extent/2);
   for (int i = tickDegMin; i <= tickDegMax; i += tickDegIncrement) {
      p.save();
      p.rotate(i + DIAL_OFFSET);
      p.drawLine(extent*0.4, 0, extent*0.48, 0);
      if (i == 0 || i == tickDegMax) {
         QString value = QString::number(i);//(i/tickDegIncrement) * tickIncrement);
         p.drawText(extent*0.6, 0, value);
      }
      p.restore();
   }
   
   // Draw the needle
   p.setPen(Qt::NoPen);
   p.setBrush(QColor(0, 128, 0, 120));
   p.rotate(value() + DIAL_OFFSET);
   QPolygon polygon;
   polygon << QPoint(-extent*0.05, extent*0.05)
           << QPoint(-extent*0.05, -extent*0.05)
           << QPoint(extent*0.46, 0);
   p.drawPolygon(polygon);
}

