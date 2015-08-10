/* 
 * File:   enginedial.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on July 31, 2015, 1:26 PM
 */

#include "enginedial.h"
#include "enginecontroller.h"

#include <QPainter>
#include <QPolygon>
#include <QTimer>
#include <QDebug>


EngineDial::EngineDial(EngineController* _engC, int _engNum, EngineDialType _type, int _min, int _max, int _value, QWidget* _parent)
: QDial(_parent),
  engC(_engC),
  type(_type),
  valueMax(_max),
  valueMin(_min),
  engNum(_engNum)
{
   int stepSize = 0;
   if (type == DIAL_OIL_TEMP) {
      typeText = "Oil T";
      valueTextBase = QString("_X_ %1C").arg(QChar(0260));
      stepSize = 10;
      
      connect(engC, &EngineController::oilTUpdate, this, &EngineDial::setValue);
   }
   else if (type == DIAL_OIL_PRESSURE) {
      typeText = "Oil P";
      valueTextBase = "_X_ psi";
      stepSize = 10;
      
      connect(engC, &EngineController::oilPUpdate, this, &EngineDial::setValue);
   }
   else if (type == DIAL_ENG_RPM) {
      typeText = "ENG RPM";
      valueTextBase = "_X_ Hz";
      stepSize = 200;
      
      connect(engC, &EngineController::engRPMUpdate, this, &EngineDial::setValue);
   }
   else if (type == DIAL_PROP_RPM) {
      typeText = "Prop RPM";
      valueTextBase = "_X_ Hz";
      stepSize = 200;
      
      connect(engC, &EngineController::propRPMUpdate, this, &EngineDial::setValue);
   }
   else if (type == DIAL_EPR) {
      typeText = "EPR";
      valueTextBase = "_X_ part";
      stepSize = 1;
      
      connect(engC, &EngineController::eprUpdate, this, &EngineDial::setValue);
      QTimer* repaintTimer = new QTimer(this);
      connect(repaintTimer, SIGNAL(timeout()), this, SLOT(update()));
      repaintTimer->start(1000);
   }
   else if (type == DIAL_EGT) {
      typeText = "EGT";
      valueTextBase = QString("_X_ %1C").arg(QChar(0260));
      stepSize = 200;
      
      connect(engC, &EngineController::egtUpdate, this, &EngineDial::setValue);
   }
   
   tickIncrement = 0.0;
   tickDegIncrement = 21;
   tickDegMin = 0;
   tickDegMax = 210;
   tickRatio = 0.0;
   
   circleBuffer = 20;
   
   if (valueMin >= valueMax) {
      qWarning() << "Warning: dial type" << type << "has invalid range:";
      qWarning() << "   ( Min =" << valueMin << ") >= ( Max =" << valueMax;
   }
   setRange(valueMin, valueMax);

   if (_value < valueMin) {
      _value = valueMin;
   }
   else if (_value > valueMax) {
      _value = valueMax;
   }
   setValue(_value, engNum);
   setWrapping(false);
   setNotchesVisible(true);
   setSingleStep(stepSize);
   setEnabled(false);
   setMinimumSize(QSize(120, 120));
}

//EngineDial::EngineDial(const EngineDial& orig)
//{
//}

EngineDial::~EngineDial()
{
}

void EngineDial::setValue(float _value, int _engNum)
{
   if (_engNum == engNum) {
//      qDebug() << "Updating ENG" << engNum << "type:" << type << "value:" << _value;
      QDial::setValue((int) _value);
      valueFloat = _value;
   }
}

void EngineDial::paintEvent(QPaintEvent*)
{
   QPainter p(this);
   int range = valueMax - valueMin;
   if (tickIncrement == 0.0) {
      tickRatio = tickDegIncrement / (tickDegMax - tickDegMin);
      tickIncrement = range * tickRatio;
   }
   
   int extent = (width() > height()) ? height() : width();
   extent -= 2*circleBuffer;
   
   p.translate((width()-extent)/2, (height()-extent)/2);
   p.setPen(QColor(105, 105, 105));
   p.setBrush(QColor(34, 34, 34));
   
   // Draw the background circle
   // Note the 15 deg offset
   p.drawArc(0, 0, extent, extent, 15*16, (tickDegMax-tickDegMin)*16);
   
   // Draw dial identification and value text
   p.setPen(QColor(105, 105, 105));
//   p.setPen(Qt::white);
   QString valueText = QString(valueTextBase);
   QString valStr;
   if (type == DIAL_EPR) {
      valStr = QString("%1").arg(valueFloat, 3, 'f', 2, '0');
   }
   else {
      valStr = QString("%1").arg(value());
   }
   valueText.replace("_X_", valStr);
   p.drawText(extent/2-4, extent/2+18, typeText);
   p.drawText(extent/2-4, extent/2+32, valueText);
   
   // Draw tickmarks
   p.setPen(QColor(0, 128, 0, 255));
   p.translate(extent/2, extent/2);
   for (int i = tickDegMin; i <= tickDegMax; i += tickDegIncrement) {
      p.save();
      p.rotate(i + DIAL_OFFSET);
      p.drawLine(extent*0.4, 0, extent*0.48, 0);
      p.restore();
   }
   
   // Draw min and max value
   p.setPen(Qt::darkGreen);
   QString valueStr = QString::number(valueMin);
   int textX = -(extent/2 + 6*valueStr.size() - 4);
   int textY = extent/2;
   p.drawText(textX, textY, valueStr);
   
   p.setPen(Qt::red);
   valueStr = QString::number(valueMax);
   textX = extent - 2*circleBuffer - 10;
   textY = +4;
   p.drawText(textX, textY, valueStr);
   
   // Draw the needle
   p.setPen(Qt::NoPen);
   double percent = (valueFloat-valueMin) / (double) range;
   if ( percent >= 0.9 ) {
      p.setBrush(QColor(255, 0, 0, 120));
   }
   else if ( percent >= 0.8 ) {
      p.setBrush(QColor(255, 255, 0, 120));
   }
   else {
      p.setBrush(QColor(0, 255, 0, 120));
   }
   
   p.save();
   float rotation = (tickDegMax-tickDegMin)*percent + DIAL_OFFSET;
   p.rotate(rotation);
   QPolygon polygon;
   polygon << QPoint(-extent*0.05, extent*0.05)
           << QPoint(-extent*0.05, -extent*0.05)
           << QPoint(extent*0.46, 0);
   p.drawPolygon(polygon);
   p.restore();
}

