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
const int DEFAULT_MAP_WIDTH  = 400;
const int DEFAULT_MAP_HEIGHT = 400;
const int MAP_PADDING = 11; // margins=11px + border=2px + extra=1px

#endif	/* MAPCONSTS_H */

