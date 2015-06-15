/* 
 * File:   mapoverlay.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 15, 2015, 2:37 PM
 */

#ifndef MAPOVERLAY_
#define MAPOVERLAY_

#include <QWidget>

class QPaintEvent;
class MapSettings;


class MapOverlay : public QWidget {
   Q_OBJECT;

public:
   MapOverlay(MapSettings* _settings, QWidget* _parent = 0);
   MapOverlay(const MapOverlay& orig) = delete;
   virtual ~MapOverlay();

public slots:
   void setZoom(int level);

protected:
   void paintEvent(QPaintEvent*);
   
private:
   MapSettings* settings;

};

#endif	/* MAPOVERLAY_ */

