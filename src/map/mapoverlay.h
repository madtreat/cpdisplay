/* 
 * File:   mapoverlay.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 15, 2015, 2:37 PM
 */

#ifndef MAPOVERLAY_
#define MAPOVERLAY_

#include <QWidget>
#include <QImage>

class QPaintEvent;

class MapSettings;
class HDDSettings;


class MapOverlay : public QWidget {
   Q_OBJECT;

public:
   MapOverlay(HDDSettings* _hddSettings, MapSettings* _mapSettings, QWidget* _parent = 0);
   MapOverlay(const MapOverlay& orig) = delete;
   virtual ~MapOverlay();

public slots:
   void setZoom(int level);
   void panToLocation(float lat, float lon);
   void setHeading(float _heading) {heading = _heading;}

protected:
   void paintEvent(QPaintEvent*);
   
private:
   HDDSettings* hddSettings;
   MapSettings* mapSettings;
   
   int heading; // current heading value in degrees
   
   // icons/images
   QImage aircraftIcon;
};

#endif	/* MAPOVERLAY_ */

