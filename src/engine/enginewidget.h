/* 
 * File:   enginewidget.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 15, 2015, 2:37 PM
 */

#ifndef ENGINEWIDGET_
#define ENGINEWIDGET_

#include <QFrame>
#include "throttlewidget.h"


class HDDSettings;
class EngineController;
class EngineDialWidget;


class EngineWidget : public QFrame {
   Q_OBJECT;

public:
   EngineWidget(HDDSettings* _hddSettings, EngineController* _engC, int _numEngines=2, QFrame* _parent = 0);
   EngineWidget(const EngineWidget& orig) = delete;
   virtual ~EngineWidget();

public slots:

private:
   HDDSettings* hddSettings;
   EngineController* engC;
   int numEngines;

   ThrottleList      throttles;
   EngineDialWidget* oilTWidget; // temp
   EngineDialWidget* oilPWidget; // pressure
   
   void setupEngineControls();
};

#endif	/* ENGINEWIDGET_ */

