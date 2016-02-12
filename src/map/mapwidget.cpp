/* 
 * File:   mapwidget.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 15, 2015, 2:36 PM
 */

#include "mapwidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QResizeEvent>

#include "qfi/ui/LayoutSquare.h"
#include "qt-maps/mapsettings.h"
#include "core/cpdsettings.h"

#include "mapconsts.h"
#include "mapcontroller.h"
#include "mapview.h"
#include "mapoverlay.h"

MapWidget::MapWidget(
  CPDSettings* _cpdSettings,
  MapSettings* _mapSettings,
  MapController* _mapC,
  ACMap* _acMap,
  QFrame* _parent
)
: QFrame(_parent),
cpdSettings(_cpdSettings),
mapSettings(_mapSettings),
mapC(_mapC),
acMap(_acMap) {
  QVBoxLayout* layout = new QVBoxLayout(this);

//  QSizePolicy sp(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
//  sp.setHeightForWidth(true);
//  setSizePolicy(sp);

  view = new MapView(cpdSettings, mapSettings, mapC, acMap);
//  view->setSizePolicy(sp);
  layout->addWidget(view);

  overlay = new MapOverlay(cpdSettings, mapSettings, acMap, view);
  overlay->setGeometry(view->geometry());
  // DO NOT ADD the overlay to the layout; it mirrors the view's geometry,
  // therefore it will be placed where it supposed to go on its own.

  // Connect CPDController-via-MapController signals to their destinations.
  connect(mapC, &MAPC::panToLocation, this, &MapWidget::panToLocation);
  connect(mapC, &MAPC::acUpdated, view, &MapView::updateAC);
  connect(mapC, &MAPC::setHeading, view, &MapView::setHeading);
  connect(mapC, &MAPC::setHeading, overlay, &MapOverlay::setHeading);

  setupControls();
  layout->addWidget(controls);

  setMinimumSize(QSize(DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT));
  resize(DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT);
  setObjectName("border");
}

//MapWidget::MapWidget(const MapWidget& orig) {
//}

MapWidget::~MapWidget() {
  delete view;
}

QSize MapWidget::sizeHint() const {
  return QSize(DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT);
}

void MapWidget::resizeEvent(QResizeEvent* event) {
  resize(event->size());
}

void MapWidget::resize(int w, int h) {
  resize(QSize(w, h));
}

void MapWidget::resize(const QSize& size) {
  QWidget::resize(size);
  int hPadding = 2 * MAP_PADDING;
  int vPadding = 3 * MAP_PADDING + BUTTON_HEIGHT;
  QSize paddedSize(size.width()-hPadding, size.height()-vPadding);
  view->resize(paddedSize);
  overlay->resize(paddedSize);
}

void MapWidget::setZoom(int level) {
  view->setZoom(level);
  overlay->setZoom(level);
}

void MapWidget::panToLocation(float lat, float lon) {
  view->panToLocation(lat, lon);
  overlay->panToLocation(lat, lon);
  overlay->repaint();
}

/*
 * Helper function for creating consistent toolbar buttons.
 */
QPushButton* MapWidget::createToolButton(QString text, bool checkable) {
  QPushButton* button = new QPushButton(text);
  button->setMinimumSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
  QSizePolicy sp(QSizePolicy::Fixed, QSizePolicy::Fixed);
  button->setSizePolicy(sp);

  if (checkable) {
    button->setCheckable(true);
  }

  return button;
}

void MapWidget::setupControls() {
  controls = new QFrame(this);
//  controls->setObjectName("border-light");
  controls->setMaximumHeight(BUTTON_HEIGHT);
  QHBoxLayout* controlsLayout = new QHBoxLayout(controls);
  controlsLayout->setContentsMargins(0, 0, 0, 0);
  controlsLayout->setSpacing(10);

//  weatherButton = createToolButton("WX", false);
//  weatherButton->setEnabled(false);
//  controlsLayout->addWidget(weatherButton);

  trafficButton = createToolButton("TFC", true);
  trafficButton->setEnabled(true);
  connect(trafficButton, SIGNAL(toggled(bool)), view,    SLOT(displayTraffic(bool)));
  connect(trafficButton, SIGNAL(toggled(bool)), overlay, SLOT(displayTraffic(bool)));
  trafficButton->setChecked(true);
  controlsLayout->addWidget(trafficButton);

  terrainButton = createToolButton("SAT", true);
  terrainButton->setEnabled(true);
  connect(terrainButton, SIGNAL(toggled(bool)), view,    SLOT(showSatMap(bool)));
  connect(terrainButton, SIGNAL(toggled(bool)), overlay, SLOT(satButtonClicked(bool)));
  terrainButton->setChecked(true);
  controlsLayout->addWidget(terrainButton);

  orientationButton = createToolButton("North Up", true);
  orientationButton->setEnabled(true);
  orientationButton->setChecked(true);
  if (mapSettings->mapOrientation() == TRACK_UP) {
     orientationButton->setChecked(false);
  }
  connect(orientationButton, SIGNAL(toggled(bool)), this, SLOT(orientationButtonClicked(bool)));
  controlsLayout->addWidget(orientationButton);

  zoomInButton = createToolButton("Zoom In", false);
  zoomInButton->setEnabled(true);
  connect(zoomInButton, SIGNAL(clicked()),    mapC, SLOT(increaseZoom()));
  // If you are at the max zoom (very close shot of ground), disable zoom in button
  connect(mapC, SIGNAL(zoomMaxReached(bool)), zoomInButton, SLOT(setDisabled(bool)));
  connect(mapC, SIGNAL(zoomEither(bool)),     zoomInButton, SLOT(setEnabled(bool)));
  controlsLayout->addWidget(zoomInButton);

  zoomOutButton = createToolButton("Zoom Out", false);
  zoomOutButton->setEnabled(true);
  connect(zoomOutButton, SIGNAL(clicked()),   mapC, SLOT(decreaseZoom()));
  // If you are at the minimum zoom (whole earth), disable zoom out button
  connect(mapC, SIGNAL(zoomMinReached(bool)), zoomOutButton, SLOT(setDisabled(bool)));
  connect(mapC, SIGNAL(zoomEither(bool)),     zoomOutButton, SLOT(setEnabled(bool)));
  controlsLayout->addWidget(zoomOutButton);

  // No longer using the toolbar directly in the QMainWindow so it can be
  // inserted into the LayoutManager.
  //   this->addToolBar(Qt::RightToolBarArea, toolbar);
}

void MapWidget::orientationButtonClicked(bool checked) {
  MapOrientation mo = checked ? NORTH_UP : TRACK_UP;
  mapC->setOrientation(mo);
}

