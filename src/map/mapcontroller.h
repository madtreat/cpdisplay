/* 
 * File:   mapcontroller.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 11, 2015, 12:33 PM
 */

#ifndef MAPCONTROLLER_H
#define MAPCONTROLLER_H

#include <QObject>

class MapController : public QObject {
   Q_OBJECT;

public:
   MapController(QObject* _parent = 0);
   MapController(const MapController& orig);
   virtual ~MapController();

public slots:

private:

};

#endif	/* MAPCONTROLLER_H */

