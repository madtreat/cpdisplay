/* 
 * File:   aircraft.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 26, 2015, 4:16 PM
 */

#include "aircraft.h"

Aircraft::Aircraft(int _id, QObject* _parent)
: QObject(_parent),
  id(_id),
  displayed(false)
{
   lat = 0.0;
   lon = 0.0;
   alt = 0.0;
   
   spd = 0.0;
   hdg = 0.0;
   rng = 0.0;
   ber = 0.0;
   
   latUpdated = false;
   lonUpdated = false;
   altUpdated = false;
}

//Aircraft::Aircraft(const Aircraft& orig)
//{
//}

Aircraft::~Aircraft()
{
}

void Aircraft::tryEmitUpdate()
{
   if (latUpdated && lonUpdated && altUpdated) {
      emit acUpdated(id);
      latUpdated = false;
      lonUpdated = false;
      altUpdated = false;
   }
}

void Aircraft::tryEmitSpdUpdate()
{
   if (spdXUpdated && spdYUpdated && spdZUpdated) {
      emit spdUpdated(id);
      spdXUpdated = false;
      spdYUpdated = false;
      spdZUpdated = false;
   }
}

void Aircraft::setLat(float _lat)
{
   lat = _lat;
   latUpdated = true;
   tryEmitUpdate();
}

void Aircraft::setLon(float _lon)
{
   lon = _lon;
   lonUpdated = true;
   tryEmitUpdate();
}

void Aircraft::setAlt(float _alt)
{
   alt = _alt;
   altUpdated = true;
   tryEmitUpdate();
}

void Aircraft::setLatLonAlt(float _lat, float _lon, float _alt)
{
   lat = _lat;
   lon = _lon;
   alt = _alt;
   
   emit acUpdated(id);
   latUpdated = false;
   lonUpdated = false;
   altUpdated = false;
}

void Aircraft::setHdg(float _hdg)
{
   hdg = _hdg;
   emit hdgUpdated(id);
}

void Aircraft::setSpdX(float _vx)
{
   spdX = _vx;
   spdXUpdated = true;
   tryEmitSpdUpdate();
}

void Aircraft::setSpdY(float _vy)
{
   spdY = _vy;
   spdYUpdated = true;
   tryEmitSpdUpdate();
}

void Aircraft::setSpdZ(float _vz)
{
   spdZ = _vz;
   spdZUpdated = true;
   tryEmitSpdUpdate();
}

void Aircraft::setRngBer(double _rng, double _ber)
{
   rng = _rng;
   ber = _ber;
   emit rngBerUpdated(id);
}
