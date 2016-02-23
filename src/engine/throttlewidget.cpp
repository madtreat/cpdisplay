/*
 * File:   throttlewidget.cpp
 * Author: madtreat
 *
 * Created on July 10, 2015, 2:46 PM
 */

#include "throttlewidget.h"
#include <QProgressBar>
#include <QLabel>

#include "enginecontroller.h"


ThrottleWidget::ThrottleWidget(
  EngineController* _engC,
  int _engNum,
  int _max,
  int _value,
  QWidget* _parent
) : QWidget(_parent),
engC(_engC),
engineNum(_engNum),
maxVal(_max),
value(_value) {
  connect(engC, &EngineController::throttleUpdate, this, &ThrottleWidget::setValue);

  throttle = new QProgressBar();
  throttle->setOrientation(Qt::Vertical);
  throttle->setTextDirection(QProgressBar::BottomToTop);
  throttle->setMinimum(0);
  throttle->setMaximum(maxVal);
  throttle->setValue(value);
  throttle->setMaximumSize(QSize(30, 300));

  layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(throttle);
}

//ThrottleWidget::ThrottleWidget(const ThrottleWidget& orig) {
//}

ThrottleWidget::~ThrottleWidget() {
}

void ThrottleWidget::setValue(int _value, int _engNum) {
  // Only update if it was meant for this engine
  if (_engNum == engineNum) {
    value = _value;
    throttle->setValue(value);
  }
}
