/* 
 * File:   enginecontroller.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 23, 2015, 12:33 PM
 */

#ifndef ENGINECONTROLLER_H
#define ENGINECONTROLLER_H

#include <QObject>

#include "enginewidget.h"


class HDDSettings;
class EngineWidget;

class EngineController : public QObject {
   Q_OBJECT;

public:
   EngineController(HDDSettings* _hddSettings, int _numThrottles, QObject* _parent = 0);
   EngineController(const EngineController& orig) = delete;
   virtual ~EngineController();
   
   EngineWidget*  getWidget()  const { return engineWidget; }

public slots:
   
signals:

private:
   HDDSettings* hddSettings;
   EngineWidget* engineWidget;
};

#endif	/* ENGINECONTROLLER_H */

