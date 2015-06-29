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

class QPaintEvent;
class QWebView;

class HDDSettings;
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

public slots:
   void startedLoading();
   void loadingProgress(int percent);
   void finishedLoading(bool success);
   
   void showCoordinates(double lat, double lon, bool saveMarker=true);
   void errorOccurred(const QString& error);
   
   void setZoom(int level);
   void panToLocation(float lat, float lon);
   void setHeading(float _heading) {heading = _heading;}

protected:
//   void paintEvent(QPaintEvent*);
   
private:
   HDDSettings*         hddSettings;
   MapSettings*         settings;
   bool                 enabled; // are the maps enabled?
   
   QWebView*            webView;
   GeocodeDataManager*  geocode;
   
   int heading; // current heading value in degrees

   QVariant evaluateJS(QString js);
   void calculateDistanceScale();
};

#endif	/* MAPVIEW_ */

