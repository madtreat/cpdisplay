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
#include <QVector>
#include <QLineF>

#include "core/mapconsts.h"
#include "core/hddsettings.h"

class QPaintEvent;

class MapSettings;


class MapOverlay : public QWidget {
   Q_OBJECT;
   typedef QVector<QLineF> TickList;

public:
   MapOverlay(HDDSettings* _hddSettings, MapSettings* _mapSettings, QWidget* _parent = 0);
   MapOverlay(const MapOverlay& orig) = delete;
   virtual ~MapOverlay();
   
   QSize sizeHint() const;
   void resize(int w, int h);
   void resize(const QSize& size);
   
   bool northUp() const { return hddSettings->mapOrientation() == NORTH_UP; }

public slots:
   void setZoom(int level);
   void panToLocation(float lat, float lon);
   void setHeading(float _heading) {heading = _heading; repaint();}

protected:
   double getDrawAngleRad(double deg);
   QLineF getLine(double deg, int from, int to, int cx=0, int cy=0);
   void initRangeTicks(int diameter=DEFAULT_MAP_WIDTH);
   
   void paintEvent(QPaintEvent*);
   void drawRangeCircle(QPainter& p);
   
private:
   HDDSettings* hddSettings;
   MapSettings* mapSettings;
   
   double heading; // current heading value in degrees
   
   TickList rangeCircleTicks; // list of pre-generated lines for when NORTH_UP
   
   // icons/images
   QImage aircraftIcon;
};

#endif	/* MAPOVERLAY_ */

