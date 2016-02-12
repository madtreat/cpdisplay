/* 
 * File:   mapcontroller.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 11, 2015, 12:33 PM
 */

#include "mapcontroller.h"

#include <QDebug>

#include "core/aircraft.h"
#include "core/cpdsettings.h"
#include "core/switchboard.h"
#include "qt-maps/mapsettings.h"


MapController::MapController(
  CPDSettings* _cpdSettings,
  SwitchBoard* _sb,
  ACMap* _acMap,
  QObject* _parent
)
: QObject(_parent),
sb(_sb),
cpdSettings(_cpdSettings),
acMap(_acMap) {
  connect(sb, &SWB::headingMagUpdate, this, &MAPC::setHeading);
  connect(sb, &SWB::latLonUpdate,     this, &MAPC::panToLocation);

  QString mapSettingsFile = cpdSettings->mapSettingsFile();
  mapSettings = new MapSettings(mapSettingsFile, this);
  mapSettings->mapOrientation();

  qDebug() << "Can enable maps?" << mapSettings->canEnableMaps();
}

//MapController::MapController(const MapController& orig) {
//}

MapController::~MapController() {
}

void MapController::setZoom(int level) {
  if (level >= ZOOM_MAX) {
    emit zoomMaxReached(true);
    return;
  }
  else if (level <= ZOOM_MIN) {
    emit zoomMinReached(true);
    return;
  }
  mapSettings->setZoom(level);
  emit updateZoom(level);
  emit zoomEither(true);
}

void MapController::increaseZoom() {
  int newLevel = mapSettings->zoom() + 1;
  setZoom(newLevel);
}

void MapController::decreaseZoom() {
  int newLevel = mapSettings->zoom() - 1;
  setZoom(newLevel);
}

void MapController::setOrientation(MapOrientation mo) {
  mapSettings->setMapOrientation(mo);
}
