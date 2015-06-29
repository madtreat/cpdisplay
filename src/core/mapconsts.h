/* 
 * File:   mapconsts.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 15, 2015, 11:47 AM
 */

#ifndef MAPCONSTS_H
#define MAPCONSTS_H

enum MapOrientation {
   NORTH_UP,
   TRACK_UP
};

const int ZOOM_MAX = 19; // Level 19 is the closest in one can zoom
const int ZOOM_MIN = 0;  // Level 0 shows the whole planet

// The default map size, and also the minimum size
const int DEFAULT_MAP_WIDTH  = 440;
const int DEFAULT_MAP_HEIGHT = 440;

#endif	/* MAPCONSTS_H */

