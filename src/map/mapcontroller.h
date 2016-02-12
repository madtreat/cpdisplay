/* 
 * File:   mapcontroller.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 11, 2015, 12:33 PM
 */

#ifndef MAPCONTROLLER_H
#define MAPCONTROLLER_H

#include <QObject>

#include "qt-maps/qtmapsconsts.h"
#include "core/aircraft.h"
#include "mapconsts.h"


class CPDSettings;
class MapSettings;

class MapController : public QObject {
  Q_OBJECT;

public:
  MapController(CPDSettings* _cpdSettings, ACMap* _acMap, QObject* _parent = 0);
  MapController(const MapController& orig) = delete;
  virtual ~MapController();

  MapSettings* getMapSettings() const {return mapSettings;}

public slots:
  void setZoom(int level);
  void increaseZoom();
  void decreaseZoom();
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

  // Notify of a change to zoom level
  void updateZoom(int level);

  // These signals are just passing info along from CPDController
  void panToLocation(float lat, float lon);
  void acUpdated(int id);
  void setHeading(float heading);

private:
  CPDSettings* cpdSettings;
  MapSettings* mapSettings;

  ACMap* acMap;
};

typedef MapController MAPC;

#endif  // MAPCONTROLLER_H

