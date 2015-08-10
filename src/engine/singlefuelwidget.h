//
//  singlefuelwidget.h
//  
//
//  Created by Madison Treat on 8/10/15.
//
//

#ifndef SINGLEFUELWIDGET_H_
#define SINGLEFUELWIDGET_H_

#include <QFrame>

class CPDSettings;
class EngineController;
class EngineDial;


class SingleFuelWidget : public QFrame {
   Q_OBJECT;

public:
   SingleFuelWidget(CPDSettings* _cpdSettings, EngineController* _engC, int _engNum, QFrame* _parent=0);
   ~SingleFuelWidget();

public slots:
   void setValuePercent(float value, int _engNum);
   void setValueVolume(float value, int _engNum);

private:
   CPDSettings* cpdSettings;
   EngineController* engC;
   int engNum;

   EngineDial* gauge;
   float valuePercent;
   float valueVolume;
};

#endif /* SINGLEFUELWIDGET_H_ */
