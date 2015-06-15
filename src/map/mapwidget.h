/* 
 * File:   mapwidget.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 15, 2015, 2:37 PM
 */

#ifndef MAPWIDGET_
#define MAPWIDGET_

#include <QWidget>
#include "mapconsts.h"


class MapSettings;
class MapView;
class MapOverlay;


class MapWidget : public QWidget {
   Q_OBJECT;

public:
   MapWidget(MapSettings* _settings, QWidget* _parent = 0);
   MapWidget(const MapWidget& orig) = delete;
   virtual ~MapWidget();

public slots:
   void setZoom(int level);
   void setOrientation(MapOrientation mo);
//   void incrementZoom();
//   void decrementZoom();
   

private:
   MapSettings* settings;
   MapView* view;
   MapOverlay* overlay;

};

#endif	/* MAPWIDGET_ */

