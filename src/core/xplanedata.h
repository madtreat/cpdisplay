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


/*
 * XPlane dataref indices for simple UDP output values.
 */
enum XPDataIndex {
   // XPlane < 10.40 or raw UDP from XPlane if selected
   FRAMERATE = 0,
   TIMES = 1,
   SIM_STATS = 2,
   
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
   
   // Throttle
   THROTTLE_COMMAND = 25,
   THROTTLE_ACTUAL = 26,
   
   // Engine/Prop
   ENG_POWER = 34,
   ENG_THRUST = 35,
   ENG_TORQUE = 36,
   ENG_RPM = 37,
   PROP_RPM = 38,
   PROP_PITCH = 39,
   PROPWASH = 40,
   
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
   
   // Comms/Nav
   COM_1_2_FREQ = 96,
   NAV_1_2_FREQ = 97,
   
   // End; for looping
   NUM_DATA_INDEXES = 134
};



/*
 * XPlane 10.40+ dataref constants and types.
 */
typedef unsigned char   xpchr;
typedef uint32_t        xpint;
typedef float           xpflt;
typedef double          xpdbl;

const int VEH_DIM = 20;
const int STR_DIM = 500;
const int ID_DIM = 5;


/*
 * XPlane 10.40+ dataref structs.  If they end in _in, they are input to xplane.
 * Similarly, if they end in _out, they are output from xplane (and input to
 * the CPDisplay app).
 */
// Length: ID_DIM
const char RREF_PREFIX[ID_DIM] = {'R', 'R', 'E', 'F', 'O'}; // request dataref from xplane
const char DREF_PREFIX[ID_DIM] = {'D', 'R', 'E', 'F', 'O'}; // set dataref to value
const char DSEL_PREFIX[ID_DIM] = {'D', 'S', 'E', 'L', 'O'}; // turn on data output indices
const char DATA_PREFIX[ID_DIM] = {'D', 'A', 'T', 'A', 'O'}; // set data output indices

// Prefix this struct with RREF_PREFIX and send to xplane host on port 49000
// RREF packets request a dataref to be output from xplane
struct xp_rref_in {
   xpint freq; // frequency of output from xplane
   xpint code; // id/code of output from xplane (for internal mapping)
   xpchr data[400]; // dataref string with optional "[xxx]" index for array vals
};

// The response packet to a xp_rref_in, sent from xplane to the CPDisplay
struct xp_rref_out {
   xpint code; // the id/code defined in xp_rref_in
   xpflt data; // output data from xplane
};

// Prefix this struct with DREF_PREFIX and send to xplane
// DREF packets tell xplane to set a value
struct xp_dref_in {
   xpflt value; // the value you are setting for the dataref
   xpchr dref_path[STR_DIM]; // the null-terminated dataref string being set
};

// Prefix this struct with DSEL_PREFIX to request UDP output for the specified
// indices in xplane.
// DSEL packets select raw UDP output indices
struct xp_dsel_in {
   xpint* data;//[400];
};

// Prefix this struct with DATA_PREFIX to set UDP output values
// DATA packets set values for a specified UDP output index
struct xp_data_in {
   xpint index;   // index of the value-set in xplane's data output screen
   xpflt data[8]; // the 8 (or less) float values you want to set at this
                  //   index in the data output screen
};



/*
 * XPlane (pre-10.40) output data.
 */
enum XPDataType {
   DOUBLE = 0,
   FLOAT,
   INT
};


// Reverse the byte order
QByteArray reverse(const QByteArray& ba);


// Output from xplane comes to this struct.
// Create an object, then call parseRawData() to fill in the values.
struct XPOutputData {
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

