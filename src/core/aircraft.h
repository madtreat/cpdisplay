/* 
 * File:   aircraft.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 26, 2015, 4:16 PM
 * 
 * This aicraft class represents any aircraft that is NOT the one being flown in
 * this cockpit.
 */

#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <QObject>
#include <QMap>


// These values are a bitmask, so you can have an AC_TYPE_JET | AC_TYPE_BOMBER
enum AircraftType {
  AC_UNKNOWN     = 0,
  // Types of engines
  AC_JET         = 1 << 0,
  AC_PISTON      = 1 << 1,
  AC_TURBOPROP   = 1 << 2,
  // Types of aircraft
  AC_AIRLINER    = 1 << 3,
  AC_ATTACK      = 1 << 4,
  AC_CARGO       = 1 << 5,
  AC_BOMBER      = 1 << 6,
  AC_EW          = 1 << 7,
  AC_FIGHTER     = 1 << 8,
  AC_GA          = 1 << 9,
  AC_GLIDER      = 1 << 10,
  AC_HELICOPTER  = 1 << 11,
  AC_RECON       = 1 << 12,
  AC_SEAPLANE    = 1 << 13,
  AC_SPACECRAFT  = 1 << 14,
  AC_TANKER      = 1 << 15,
  AC_UAV         = 1 << 16,
  AC_VTOL        = 1 << 17,
  AC_XPLANE      = 1 << 18 
};

static const int ENG_TYPES = AC_JET | AC_PISTON | AC_TURBOPROP;


class Aircraft : public QObject {
  Q_OBJECT;

public:
  Aircraft(int _id, QObject* _parent = 0);
  Aircraft(const Aircraft& orig) = delete;
  virtual ~Aircraft();

  int   getID()  const { return id; }

  float getSpd() const { return spd; }
  float getLat() const { return lat; }
  float getLon() const { return lon; }
  float getAlt() const { return alt; }
  float getHdg() const { return hdg; }
  float getRng() const { return rng; }
  float getBer() const { return ber; }

  bool hasBeenDisplayed() const { return displayed; }
  void setHasBeenDisplayed() { displayed = true; }

public slots:
  void setLat(float _lat);
  void setLon(float _lon);
  void setAlt(float _alt);
  void setLatLonAlt(float _lat, float _lon, float _alt);
  void setHdg(float _hdg);
  void setSpdX(float _vx);
  void setSpdY(float _vy);
  void setSpdZ(float _vz);

  void setRngBer(double _rng, double _ber);

signals:
  void acUpdated(int id);       // lat/lon/alt
  void rngBerUpdated(int id);   // rng/ber
  void hdgUpdated(int id);      // hdg
  void spdUpdated(int id);      // spd (after all 3 components)

private:
  AircraftType type;
  int   id;  // aircraft id
  bool  displayed;  // has this aircraft been displayed on the map yet?

  float lat;   // latitude
  float lon;   // longitude
  float alt;   // MSL altitude

  float spd;   // speed
  float spdX;  // speed x component
  float spdY;  // speed y component
  float spdZ;  // speed z component
  float hdg;   // aircraft's heading

  // These values are from a reference point of the current cockpit's position
  float rng;   // aircraft's range from this position
  float ber;   // aircraft's bearing from this position

  // Flags for updating groups of values together
  bool latUpdated;
  bool lonUpdated;
  bool altUpdated;

  bool spdXUpdated;
  bool spdYUpdated;
  bool spdZUpdated;

  void tryEmitUpdate();
  void tryEmitSpdUpdate();
};

// map of ID to Aircraft
typedef QMap<int, Aircraft*> ACMap;

#endif	/* AIRCRAFT_H */

