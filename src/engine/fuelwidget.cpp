//
//  fuelwidget.cpp
//
//
//  Created by Madison Treat on 8/10/15.
//
//

#include "fuelwidget.h"

#include <QHBoxLayout>
#include <QString>
#include <QLabel>

#include "core/cpdsettings.h"
#include "core/xplanedref.h"
#include "enginecontroller.h"
#include "singlefuelwidget.h"


FuelWidget::FuelWidget(
  CPDSettings* _cpdSettings,
  EngineController* _engC,
  int _numTanks,
  QFrame* _parent
) : cpdSettings(_cpdSettings),
engC(_engC),
numTanks(_numTanks) {
  connect(engC, &ENGC::numTanksUpdate, this, &FuelWidget::setNumTanks);
  setupFuelGauges();

//  setMinimumSize(QSize(65, 180));
  setObjectName("border");
}

FuelWidget::~FuelWidget() {
}

void FuelWidget::setupFuelGauges() {
  QHBoxLayout* layout = new QHBoxLayout(this);

  layout->addStretch();
  for (int i = 0; i < MAX_NUM_FUEL_TANKS; i++) {
    SingleFuelWidget* fgauge = new SingleFuelWidget(cpdSettings, engC, i);
    fgauge->setVisible(i < numTanks);

    fuelGauges.append(fgauge);
    layout->addWidget(fgauge);
  }
  layout->addStretch();
}

void FuelWidget::setNumTanks(int _tanks) {
  numTanks = _tanks;
}
