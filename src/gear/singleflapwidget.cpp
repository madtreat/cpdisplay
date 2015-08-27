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


SingleFlapWidget::SingleFlapWidget(CPDSettings* _settings, GearController* _gearC, QFrame* _parent)
: QFrame(_parent),
  settings(_settings),
  gearC(_gearC)
{
   setMinimumWidth(38);
   setMaximumWidth(40);

   QVBoxLayout* layout = new QVBoxLayout(this);
   layout->setContentsMargins(0, 0, 0, 0);
   QHBoxLayout* center = new QHBoxLayout();
   center->setContentsMargins(0, 0, 0, 0);

   slider = new QSlider(Qt::Vertical);
   slider->setTickInterval(10);
   slider->setTickPosition(QSlider::TicksBothSides);
   slider->setValue(0);
   slider->setMinimum(0);
   slider->setMaximum(100);
   connect(slider, &QSlider::valueChanged, gearC, &GEARC::notifyXPlaneFlap1);
   connect(slider, &QSlider::valueChanged, this, &SingleFlapWidget::updateLabel);
   connect(slider, &QSlider::valueChanged, this, &SingleFlapWidget::flapUpdate);

   label = new QLabel("0%");
   label->setAlignment(Qt::AlignHCenter);

   center->addStretch();
   center->addWidget(slider);
   center->addStretch();

   layout->addLayout(center);
   layout->addWidget(label);
}

SingleFlapWidget::~SingleFlapWidget()
{
}

void SingleFlapWidget::updateLabel(int percent)
{
   label->setText(QString("%1%").arg(percent));
}
