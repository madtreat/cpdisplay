/* 
 * File:   mapcontroller.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 11, 2015, 12:33 PM
 */

#ifndef MAPCONTROLLER_H
#define MAPCONTROLLER_H

#include <QObject>

#include "core/aircraft.h"
#include "core/mapconsts.h"
#include "mapwidget.h"


class HDDSettings;
class MapSettings;
class MapWidget;
class MapView;
class MapOverlay;

class MapController : public QObject {
   Q_OBJECT;

public:
   MapController(HDDSettings* _hddSettings, ACMap* _acMap, QObject* _parent = 0);
   MapController(const MapController& orig) = delete;
   virtual ~MapController();
   
   MapWidget*  getWidget()  const { return mapWidget; }
   MapView*    getMapView() const { return mapWidget->getMapView(); }
   MapOverlay* getOverlay() const { return mapWidget->getOverlay(); }

public slots:
   void setZoom(int level);
   void increaseZoom();
   void decreaseZoom();
   void panToLocation(float lat, float lon);
   void setOrientation(MapOrientation mo);
   
signals:
   /*
    * These signals use a bool argument to interact directly with QPushButtons'
    * setEnabled(bool) slots. 
    * 
    * Use "true" ALWAYS because that is what you want to satisfy "disable" or
    * "enable" parameters.
    */
   void zoomMaxReached(bool disable);
   void zoomMinReached(bool disable);
   void zoomEither(bool enable);

private:
   HDDSettings* hddSettings;
   MapSettings* settings;
   MapWidget* mapWidget;
   
   ACMap* acMap;
   
   MapOrientation orientation;
};

#endif	/* MAPCONTROLLER_H */

