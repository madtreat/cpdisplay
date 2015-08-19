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
   DIAL_ENG_RPM,
   DIAL_PROP_RPM,
   DIAL_EPR,
   DIAL_EGT,
   DIAL_FUEL
};


class ThrottleWidget;
typedef QList<ThrottleWidget*> ThrottleList;

class SingleEngineWidget;
typedef QList<SingleEngineWidget*> Engines;

class SingleFuelWidget;
typedef QList<SingleFuelWidget*> FuelGauges;

#endif	/* ENGCONSTS_H */

