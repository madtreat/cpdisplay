/*
 * File:   enginewidget.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 15, 2015, 2:37 PM
 */

#ifndef ENGINEWIDGET_
#define ENGINEWIDGET_

#include <QFrame>

#include "engineconsts.h"

class CPDSettings;
class EngineController;


class EngineWidget : public QFrame {
  Q_OBJECT;

public:
  EngineWidget(CPDSettings* _cpdSettings, EngineController* _engC, int _numEngines=1, QFrame* _parent = 0);
  EngineWidget(const EngineWidget& orig) = delete;
  virtual ~EngineWidget();

public slots:
  void setNumEngines(int _eng);

private:
  CPDSettings* cpdSettings;
  EngineController* engC;
  int numEngines;

  Engines engines;

  void setupEngineControls();
};

#endif  // ENGINEWIDGET_

