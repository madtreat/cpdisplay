/* 
 * File:   mapcontroller.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 11, 2015, 12:33 PM
 */

#ifndef MAPCONTROLLER_H
#define MAPCONTROLLER_H

#include <QObject>
#include "mapconsts.h"

class MapSettings;
class MapWidget;

class MapController : public QObject {
   Q_OBJECT;

public:
   MapController(QObject* _parent = 0);
   MapController(const MapController& orig) = delete;
   virtual ~MapController();
   
   MapWidget* getWidget() const { return mapWidget; }

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

private:
   MapSettings* settings;
   MapWidget* mapWidget;
   
   MapOrientation orientation;
};

#endif	/* MAPCONTROLLER_H */

