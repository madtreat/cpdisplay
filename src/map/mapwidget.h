/* 
 * File:   MapWidget.
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 15, 2015, 11:21 AM
 */

#ifndef MAPWIDGET_
#define MAPWIDGET_

#include <QWidget>
#include "mapconsts.h"

class QWebView;

class MapSettings;
class GeocodeDataManager;


class MapWidget : public QWidget {
   Q_OBJECT;

public:
   MapWidget(MapSettings* _settings, QWidget* _parent = 0);
   MapWidget(const MapWidget& orig) = delete;
   virtual ~MapWidget();

public slots:
   void showCoordinates(double lat, double lon, bool saveMarker=true);
   void errorOccurred(const QString& error);
   
   void updateZoomLevel(int level);
   void incrementZoom();
   void decrementZoom();
   
   void updateOrientation(MapOrientation mo);

private:
   MapSettings*         settings;
   bool                 enabled; // are the maps enabled?
   
   QWebView*            mapView;
   GeocodeDataManager*  geocode;
   
   int                  currentZoom;
   MapOrientation       currentOrientation;
};

#endif	/* MAPWIDGET_ */

