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

#include "engineconsts.h"


class EngineDial : public QDial {
   Q_OBJECT;
   
   static const int DIAL_OFFSET = 135;

public:
   EngineDial(EngineDialType _type, QWidget* _parent = 0);
   EngineDial(const EngineDial& orig) = delete;
   virtual ~EngineDial();

public slots:
   
protected:
   void paintEvent(QPaintEvent*);

private:
   EngineDialType type;
   QString typeText;
   QString valueTextBase;
   
   double tickIncrement; // each tick represents X value
   int tickDegIncrement; // each tick represents X degrees
   int tickDegMin;
   int tickDegMax;
   
   int circleBuffer; // buffer from edge of box to circle

};

#endif	/* ENGINEDIAL_H */

