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
#include "core/mapconsts.h"
#include "core/hddsettings.h"

class QPaintEvent;
class QWebView;

//class HDDSettings;
class MapSettings;
class GeocodeDataManager;


class MapView : public QWidget {
   Q_OBJECT;

public:
   MapView(HDDSettings* _hddSettings, MapSettings* _settings, QWidget* _parent = 0);
   MapView(const MapView& orig) = delete;
   virtual ~MapView();
   
   QSize sizeHint() const;
   void resize(int w, int h);
   void resize(const QSize& size);
   
   bool northUp() const { return hddSettings->mapOrientation() == NORTH_UP; }

public slots:
   void startedLoading();
   void loadingProgress(int percent);
   void finishedLoading(bool success);
   
   void showCoordinates(double lat, double lon, bool saveMarker=true);
   void errorOccurred(const QString& error);
   
   void setZoom(int level);
   void panToLocation(float lat, float lon);
   void setHeading(float _heading) {heading = _heading;}
   
   void setOrientation(MapOrientation mo);

protected:
//   void paintEvent(QPaintEvent*);
   void render();
   
private:
   HDDSettings*         hddSettings;
   MapSettings*         settings;
   bool                 enabled; // are the maps enabled?
   
   QWebView*            webView;
   GeocodeDataManager*  geocode;
   
   double heading; // current heading value in degrees
//   bool   northUp; // is NORTH_UP currently selected?

   QVariant evaluateJS(QString js);
   void calculateDistanceScale();
};

#endif	/* MAPVIEW_ */

