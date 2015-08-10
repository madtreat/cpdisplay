//
//  fuelwidget.h
//  
//
//  Created by Madison Treat on 8/10/15.
//
//

#ifndef FUELWIDGET_H_
#define FUELWIDGET_H_

#include <QFrame>

#include "engineconsts.h"

class CPDSettings;
class EngineController;


class FuelWidget : public QFrame {
   Q_OBJECT;

public:
   FuelWidget(CPDSettings* _cpdSettings, EngineController* _engC, int _numEngines=2, QFrame* _parent=0);
   FuelWidget(const FuelWidget& orig) = delete;
   virtual ~FuelWidget();

private:
   CPDSettings* cpdSettings;
   EngineController* engC;
   int numEngines;

   FuelGauges fuelGauges;

   void setupFuelGauges();
};

#endif /* FUELWIDGET_H_ */
