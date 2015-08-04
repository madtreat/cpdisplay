/* 
 * File:   engconsts.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on July 31, 2015, 2:39 PM
 */

#ifndef ENGCONSTS_H
#define ENGCONSTS_H

#include <QList>


enum EngineDialType {
   DIAL_OIL_TEMP,
   DIAL_OIL_PRESSURE,
   DIAL_ENG_RPM
};


class ThrottleWidget;
typedef QList<ThrottleWidget*> ThrottleList;

class SingleEngineWidget;
typedef QList<SingleEngineWidget*> Engines;

#endif	/* ENGCONSTS_H */

