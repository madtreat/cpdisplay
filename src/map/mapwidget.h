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

#include "mapconsts.h"

class QResizeEvent;
class QPushButton;

class MapSettings;
class MapView;
class MapOverlay;

class CPDSettings;
class MapController;


class MapWidget : public QFrame {
  Q_OBJECT;

  static const int BUTTON_WIDTH = 75;
  static const int BUTTON_HEIGHT = 30;

public:
  MapWidget(CPDSettings* _cpdSettings, MapSettings* _mapSettings, MapController* _mapC, ACMap* _acMap, QFrame* _parent = 0);
  MapWidget(const MapWidget& orig) = delete;
  virtual ~MapWidget();

  QSize sizeHint() const;
  void resize(int w, int h);
  void resize(const QSize& size);

  MapView*    getMapView() const { return view; }
  MapOverlay* getOverlay() const { return overlay; }

public slots:
  void orientationButtonClicked(bool checked);

  void setZoom(int level);
  void panToLocation(float lat, float lon);

protected:
  void resizeEvent(QResizeEvent* event);

private:
  CPDSettings* cpdSettings;
  MapSettings* mapSettings;

  MapController* mapC;
  MapView* view;
  MapOverlay* overlay;

  // Map control buttons and encompasing widget
  QFrame*        controls;
  QPushButton*   weatherButton;
  QPushButton*   trafficButton;
  QPushButton*   terrainButton;
  QPushButton*   orientationButton; // map orientation, checked = NORTH_UP
  QPushButton*   zoomInButton;
  QPushButton*   zoomOutButton;
  //   QPushButton*    homeButton;

  ACMap* acMap;

  QPushButton* createToolButton(QString text, bool checkable);
  void setupControls();
};

#endif  // MAPWIDGET_

