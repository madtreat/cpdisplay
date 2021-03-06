/*
 * File:   enginewidget.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 15, 2015, 2:36 PM
 */

#include "enginewidget.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

#include "core/cpdsettings.h"
#include "core/xplanedref.h"
#include "enginecontroller.h"
#include "singleenginewidget.h"


EngineWidget::EngineWidget(
  CPDSettings* _cpdSettings,
  EngineController* _engC,
  int _numEngines,
  QFrame* _parent
) : QFrame(_parent),
cpdSettings(_cpdSettings),
engC(_engC),
numEngines(_numEngines) {
  connect(engC, &ENGC::numEnginesUpdate, this, &EngineWidget::setNumEngines);
  setupEngineControls();

  setMinimumSize(QSize(65, 180));
  setObjectName("border");
}

//EngineWidget::EngineWidget(const EngineWidget& orig) {
//}

EngineWidget::~EngineWidget() {
}

void EngineWidget::setupEngineControls() {
  QHBoxLayout* layout = new QHBoxLayout(this);
//  layout->setContentsMargins(0, 0, 0, 0);

  layout->addStretch();
  // This loop will create throttles in pairs, to keep them grouped better on the main layout
  for (int i = 0; i < MAX_NUM_ENGINES; i++) {
    SingleEngineWidget* eng = new SingleEngineWidget(cpdSettings, engC, i);
    eng->setVisible(i < numEngines);

    engines.append(eng);
    layout->addWidget(eng);
  }

  layout->addStretch();
}

void EngineWidget::setNumEngines(int _eng) {
  // if (numEngines != _eng) {
  for (int i = 0; i < MAX_NUM_ENGINES; i++) {
    engines.at(i)->setVisible(i < _eng);
  }
  //}
  numEngines = _eng;
}
