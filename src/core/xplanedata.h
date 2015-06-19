/* 
 * File:   xplanedata.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 18, 2015, 10:50 AM
 */

#ifndef XPLANEDATA_H
#define XPLANEDATA_H


#include <QByteArray>
#include <QList>
#include <QVariant>
#include <QDebug>

enum XPDataIndex {
   FRAMERATE = 0,
   
   SPEEDS = 3,
   MACH_VVI_GLOAD = 4,
   
   ATMO_WEATHER = 5,
   ATMO_AIRCRAFT = 6,
   SYS_PRESSURE = 7,
   
   WING_SWEEP_THRUST_VEC = 12,
   TRIM_FLAP_SLAT_SBRAKES = 13,
   GEAR_BRAKES = 14,
   
   ANG_MOMENTS = 15,
   ANG_VEL = 16,
   PITCH_ROLL_HEADINGS = 17,
   AOA_SIDESLIP_PATHS = 18,
   MAG_COMPASS = 19,
   
   // This Aircraft
   LAT_LON_ALT = 20,
   LOC_VEL_DIST_TRAVELED = 21,
   
   // All Aircraft
   ALL_LAT = 22,
   ALL_LON = 23,
   ALL_ALT = 24,
   
   ENG_POWER = 34,
   ENG_THRUST = 35,
   ENG_TORQUE = 36,
   ENG_RPM = 37,
   PROP_RPM = 38,
   PROP_PITCH = 39,
   PROPWASH_JETWASH = 40,
   
   N1 = 41,
   N2 = 42,
   MP = 43,
   EPR = 44,
   FF = 45,
   ITT = 46,
   EGT = 47,
   CHT = 48,
   
   OIL_PRESSURE = 49,
   OIL_TEMP = 50,
   FUEL_PRESSURE = 51,
   GENERATOR_AMP = 52,
   BATTERY_AMP = 53,
   BATTERY_VOLT = 54,
   
   // On/Off
//   ELEC_FUEL_PUMP = 55,
   
   // End; for loops
   NUM_DATA_INDEXES
};


enum XPDataType {
   DOUBLE = 0,
   FLOAT,
   INT
};


// Reverse the byte order
QByteArray reverse(const QByteArray& ba);


// Create an object, then call parseRawData() to fill in the values.
struct XPData {
   XPDataIndex index;
   XPDataType  type;
   // There should be 8 integer or double values
   QList<QVariant> values;
   
   void parseRawData(QByteArray raw);
//   friend QDebug& operator<<(QDebug& os, const XPData& data);
   void print();
};


//typedef int XP_ALT;

#endif	/* XPLANEDATA_H */

