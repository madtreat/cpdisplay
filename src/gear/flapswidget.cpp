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
   setMinimumWidth(40);// NOTE: depends on flapReal included
   setMaximumWidth(120);

   QVBoxLayout* layout = new QVBoxLayout(this);
   QHBoxLayout* center = new QHBoxLayout();
   center->setContentsMargins(0, 0, 0, 0);

   QLabel* label = new QLabel("Flaps");
   label->setAlignment(Qt::AlignHCenter);
   label->setObjectName("border-light");

   flapHandle = new SingleFlapWidget(settings, gearC, FLAP_HANDLE);
   flapReal = new SingleFlapWidget(settings, gearC, FLAP_REAL);

   center->addStretch();
   center->addWidget(flapHandle);
   //center->addWidget(flapReal); // I cannot figure out which dataref this is
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
