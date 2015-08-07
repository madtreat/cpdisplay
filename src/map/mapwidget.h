/* 
 * File:   mapwidget.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 15, 2015, 2:37 PM
 */

#ifndef MAPWIDGET_
#define MAPWIDGET_

#include <QFrame>
#include "core/aircraft.h"
#include "core/mapconsts.h"

#include "mapview.h"
#include "mapoverlay.h"

class QResizeEvent;

class MapSettings;
//class MapView;
//class MapOverlay;

class CPDSettings;


class MapWidget : public QFrame {
   Q_OBJECT;

public:
   MapWidget(CPDSettings* _cpdSettings, MapSettings* _mapSettings, ACMap* _acMap, QFrame* _parent = 0);
   MapWidget(const MapWidget& orig) = delete;
   virtual ~MapWidget();
   
   QSize sizeHint() const;
   void resize(int w, int h);
   void resize(const QSize& size);

   MapView*    getMapView() const { return view; }
   MapOverlay* getOverlay() const { return overlay; }

public slots:
   void setZoom(int level);
   void panToLocation(float lat, float lon);
   void setOrientation(MapOrientation mo);
   
protected:
   void resizeEvent(QResizeEvent* event);

private:
   CPDSettings* cpdSettings;
   MapSettings* mapSettings;
   MapView* view;
   MapOverlay* overlay;
   
   ACMap* acMap;

};

#endif	/* MAPWIDGET_ */

