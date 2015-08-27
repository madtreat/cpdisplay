/*
 * File:   flapswidget.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on August 26, 2015
 */

#include "flapswidget.h"

#include <QSlider>
#include <QString>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "core/cpdsettings.h"
#include "gearcontroller.h"
#include "singleflapwidget.h"


FlapsWidget::FlapsWidget(CPDSettings* _settings, GearController* _gearC, QFrame* _parent)
: QFrame(_parent),
  settings(_settings),
  gearC(_gearC)
{
   setObjectName("border");
   setMinimumWidth(80);
   setMaximumWidth(120);

   QVBoxLayout* layout = new QVBoxLayout(this);
   QHBoxLayout* center = new QHBoxLayout(this);
   center->setContentsMargins(0, 0, 0, 0);

   QLabel* label = new QLabel("Flaps");
   label->setAlignment(Qt::AlignHCenter);
   label->setObjectName("border-light");

   flap1 = new SingleFlapWidget(settings, gearC);
   flap2 = new SingleFlapWidget(settings, gearC);

   center->addStretch();
   center->addWidget(flap1);
   center->addWidget(flap2);
   center->addStretch();

   layout->addWidget(label);
   layout->addLayout(center);
}

FlapsWidget::~FlapsWidget()
{
}

//void FlapsWidget::setFlaps()
//{
//}
