/* 
 * File:   geodist.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on July 3, 2015, 12:08 PM
 */

#ifndef GEODIST_H
#define GEODIST_H

namespace geo {
   double distance(double lat1, double lon1, double lat2, double lon2, char unit='N');
   double bearing(double lat1, double lon1, double lat2, double lon2, char unit='N');
}

double deg2rad(double deg);
double rad2deg(double rad);

#endif	/* GEODIST_H */

