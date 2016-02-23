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

class QLabel;

class CPDSettings;
class EngineController;
class EngineDial;


class SingleFuelWidget : public QFrame {
  Q_OBJECT;

public:
  SingleFuelWidget(CPDSettings* _cpdSettings, EngineController* _engC, int _tankNum, QFrame* _parent=0);
  ~SingleFuelWidget();

public slots:
  void setValuePercent(float value, int _tankNum);
  void setValueVolume(float value, int _tankNum);
  void setFF(float value, int _tankNum);

private:
  CPDSettings* cpdSettings;
  EngineController* engC;
  int tankNum;

  EngineDial* gauge;
  float valuePercent;
  float valueVolume;


  QLabel* fuelFlow;
  QString ffUnits;
};

#endif  // SINGLEFUELWIDGET_H_

