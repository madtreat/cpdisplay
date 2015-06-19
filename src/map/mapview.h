/* 
 * File:   MapView.
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 15, 2015, 11:21 AM
 * 
 * The MapView class contains the raw map from Google.
 */

#ifndef MAPVIEW_
#define MAPVIEW_

#include <QWidget>
#include "mapconsts.h"

class QWebView;

class MapSettings;
class GeocodeDataManager;


class MapView : public QWidget {
   Q_OBJECT;

public:
   MapView(MapSettings* _settings, QWidget* _parent = 0);
   MapView(const MapView& orig) = delete;
   virtual ~MapView();

public slots:
   void startedLoading();
   void loadingProgress(int percent);
   void finishedLoading(bool success);
   
   void showCoordinates(double lat, double lon, bool saveMarker=true);
   void errorOccurred(const QString& error);
   
   void setZoom(int level);
   void panToLocation(float lat, float lon);

private:
   MapSettings*         settings;
   bool                 enabled; // are the maps enabled?
   
   QWebView*            webView;
   GeocodeDataManager*  geocode;
   
};

#endif	/* MAPVIEW_ */

