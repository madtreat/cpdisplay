/* 
 * File:   enginedial.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on July 31, 2015, 1:26 PM
 */

#ifndef ENGINEDIAL_H
#define ENGINEDIAL_H

#include <QDial>
#include <QWidget>
#include <QColor>

#include "core/xplanedref.h"
#include "engineconsts.h"


class EngineController;


class EngineDial : public QDial {
   Q_OBJECT;
   
   static const int DIAL_OFFSET = 135;

public:
   EngineDial(EngineController* _engC, int _engNum, EngineDialType _type, int _min, int _max, int _value=0, QWidget* _parent=0);
   EngineDial(const EngineDial& orig) = delete;
   virtual ~EngineDial();

public slots:
   void setValue(float value, int _engNum);
   void setLimit(float limit, LimitType lt);
   
protected:
   void paintEvent(QPaintEvent*);

private:
   EngineController* engC;
   EngineDialType    type;
   int               engNum;
   
   int valueMin;
   int valueMax;
   float valueFloat;
   
   int limitGreenLo;
   int limitGreenHi;
   int limitYellowLo;
   int limitYellowHi;
   int limitRedLo;
   int limitRedHi;

   double tickIncrement; // each tick represents X value
   double tickRatio;     // ratio of value to rotation degrees
   int tickDegIncrement; // each tick represents X rotation degrees
   int tickDegMin;
   int tickDegMax;
   
   int circleBuffer; // buffer from edge of box to circle
   
   QString typeText;
   QString valueTextBase;

   QColor tickColor;
   QColor dialColor;
   QColor bgColor;

   QColor grnColor;
   QColor ylwColor;
   QColor redColor;

};

#endif	/* ENGINEDIAL_H */

