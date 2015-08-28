/*
 * File:   singleflapwidget.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on August 26, 2015
 */

#include "singleflapwidget.h"

#include <QSlider>
#include <QString>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "core/cpdsettings.h"
#include "gearcontroller.h"


SingleFlapWidget::SingleFlapWidget(CPDSettings* _settings, GearController* _gearC, FlapWidgetType _type, QFrame* _parent)
: QFrame(_parent),
  settings(_settings),
  gearC(_gearC),
  type(_type)
{
   setMinimumWidth(38);
   setMaximumWidth(40);

   QVBoxLayout* layout = new QVBoxLayout(this);
   layout->setContentsMargins(0, 0, 0, 0);
   QHBoxLayout* center = new QHBoxLayout();
   center->setContentsMargins(0, 0, 0, 0);

//   QLabel* typeLabel;
   if (type == FLAP_HANDLE) {
//      typeLabel = new QLabel("Handle");
   }
   else {
//      typeLabel = new QLabel("Real");
   }

   slider = new QSlider(Qt::Vertical);
   slider->setTickInterval(10);
   slider->setTickPosition(QSlider::TicksBothSides);
   slider->setValue(0);
   slider->setMinimum(0);
   slider->setMaximum(100);
   if (type == FLAP_HANDLE) {
      connect(slider, &QSlider::valueChanged, gearC, &GEARC::notifyXPlaneFlapHandle);
      connect(gearC, &GEARC::flapHandleUpdate, this, &SingleFlapWidget::setFlap);
   }
   else {
      slider->setEnabled(false);
      connect(gearC, &GEARC::flapUpdate, this, &SingleFlapWidget::setFlap);
   }
   connect(slider, &QSlider::valueChanged, this, &SingleFlapWidget::updateLabel);

   valueLabel = new QLabel("0%");
   valueLabel->setAlignment(Qt::AlignHCenter);

   center->addStretch();
   center->addWidget(slider);
   center->addStretch();

//   layout->addWidget(typeLabel);
   layout->addLayout(center);
   layout->addWidget(valueLabel);
}

SingleFlapWidget::~SingleFlapWidget()
{
}

void SingleFlapWidget::setFlap(float percent)
{
   slider->setValue(percent*100);
}

void SingleFlapWidget::updateLabel(int percent)
{
   valueLabel->setText(QString("%1%").arg(percent));
}

