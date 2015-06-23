/* 
 * File:   commscontroller.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 23, 2015, 12:33 PM
 */

#ifndef COMMSCONTROLLER_H
#define COMMSCONTROLLER_H

#include <QObject>

#include "commswidget.h"


class HDDSettings;
class CommsWidget;

class CommsController : public QObject {
   Q_OBJECT;

public:
   CommsController(HDDSettings* _hddSettings, QObject* _parent = 0);
   CommsController(const CommsController& orig) = delete;
   virtual ~CommsController();
   
   CommsWidget*  getWidget()  const { return commsWidget; }

public slots:
   
signals:

private:
   HDDSettings* hddSettings;
   CommsWidget* commsWidget;
};

#endif	/* COMMSCONTROLLER_H */

