/* 
 * File:   commsline.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on July 23, 2015, 12:01 PM
 */

#include "commsline.h"

#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QDoubleSpinBox>

#include "commscontroller.h"


CommsLine::CommsLine(CommsController* _comC, CommType _commType, QWidget* _parent)
: QWidget(_parent),
  comC(_comC),
  type(_commType),
  activeJustChanged(false),
  standbyJustChanged(false),
  justInitialized(true)
{
   connect(comC, &CommsController::comUpdate, this, &CommsLine::setFreqs);
   
   activeFreq = 0.0;
   standbyFreq = 0.0;
   
   QString typeString;
   if (type & COM1) {
      typeString = "COM 1:";
   }
   else if (type & COM2) {
      typeString = "COM 2:";
   }
   else if (type & NAV1) {
      typeString = "NAV 1:";
   }
   else if (type & NAV2) {
      typeString = "NAV 2:";
   }
   typeLabel = new QLabel(typeString);
   typeLabel->setMinimumHeight(20);

   activeSB = createFreqSpinBox();
   standbySB = createFreqSpinBox();
   connect(activeSB,  SIGNAL(valueChanged(double)), this, SLOT(activeChanged(double)));
   connect(standbySB, SIGNAL(valueChanged(double)), this, SLOT(standbyChanged(double)));
   
//   actUp = new QPushButton("^");
//   actDown = new QPushButton("v");
//   standbyUp = new QPushButton("^");
//   standbyDown = new QPushButton("v");
   
   swap = new QPushButton("< Swap >");
   connect(swap, &QPushButton::clicked, this, &CommsLine::swapActive);
   
   QHBoxLayout* layout = new QHBoxLayout(this);
   layout->setContentsMargins(0, 0, 0, 0);
   layout->addWidget(typeLabel);
//   layout->addWidget(actDown);
   layout->addWidget(activeSB);
//   layout->addWidget(actUp);
   layout->addWidget(swap);
//   layout->addWidget(standbyDown);
   layout->addWidget(standbySB);
//   layout->addWidget(standbyUp);

//   QFormLayout* lineLayout = new QFormLayout(this);
//   lineLayout->setContentsMargins(0, 0, 0, 0);
//   lineLayout->addRow(typeString, layout);

   justInitialized = false;
}

//CommsLine::CommsLine(const MapSettings& orig)
//{
//}

CommsLine::~CommsLine()
{
}

QDoubleSpinBox* CommsLine::createFreqSpinBox()
{
   QDoubleSpinBox* spin = new QDoubleSpinBox();
   bool com = (type & COM1) | (type & COM2);
   spin->setMinimum(com ? 118 : 108);
   spin->setMaximum(com ? 137 : 118);
   spin->setSingleStep(com ? 0.025 : 0.05);
   spin->setWrapping(true);
   spin->setMinimumHeight(20);
   spin->setMinimumWidth(90);
   return spin;
}

QString CommsLine::formatFreq(float freq)
{
   QString str("%1");
   str = str.arg(freq);
   str.insert(3, ".");
   return str;
}

void CommsLine::setActiveValue(float freq)
{
   // if (!activeJustChanged) {
      activeFreq = freq;
      activeSB->blockSignals(true);
      activeSB->setValue(freq / 100);
      activeSB->blockSignals(false);
      activeJustChanged = false; // only ignore the first signal after manually changing
   // }
}

void CommsLine::setStandbyValue(float freq)
{
   // if (!standbyJustChanged) {
      standbyFreq = freq;
      standbySB->blockSignals(true);
      standbySB->setValue(freq / 100);
      standbySB->blockSignals(false);
      standbyJustChanged = false; // only ignore the first signal after manually changing
   // }
}

void CommsLine::swapActive()
{
   float activeFreq2 = activeFreq;
   setActiveValue(standbyFreq);
   setStandbyValue(activeFreq2);
   
   // activeJustChanged = true;
   // standbyJustChanged = true;
   comC->comSwapped(type);
}

void CommsLine::setFreqs(CommType ct, float active, float standby)
{
   // Only update if the update was for this type
   if (ct & type) {
      setActiveValue(active);
      setStandbyValue(standby);
   }
}

void CommsLine::activeChanged(double freq)
{
   if (!justInitialized) {
      activeJustChanged = true;
      comC->notifyXPlaneComms((CommType) (type | ACTIVE), (float) 100*freq);
   }
}

void CommsLine::standbyChanged(double freq)
{
   if (!justInitialized) {
      standbyJustChanged = true;
      comC->notifyXPlaneComms((CommType) (type | STANDBY), (float) 100*freq);
   }
}
