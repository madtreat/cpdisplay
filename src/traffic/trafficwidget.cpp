/* 
 * File:   trafficwidget.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 15, 2015, 2:36 PM
 */

#include "trafficwidget.h"

#include <QFormLayout>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "core/aircraft.h"
#include "core/cpdsettings.h"
#include "trafficcontroller.h"


TrafficWidget::TrafficWidget(CPDSettings* _cpdSettings, TrafficController* _tfcC, ACMap* _acMap, QFrame* _parent)
: QFrame(_parent),
  cpdSettings(_cpdSettings),
  tfcC(_tfcC),
  acMap(_acMap),
  currentID(1)
{
   connect(this, &TrafficWidget::displayedACChanged, tfcC, &TFCC::updateCurrentAC);
   connect(tfcC, &TFCC::displayedACUpdated, this, &TrafficWidget::refreshAC);

   if (acMap->contains(currentID)) {
      currentAC = acMap->value(currentID);
   }
   else {
      currentAC = NULL;
   }
   setupTrafficControls();
   
//   setMinimumSize(QSize(320, 320));
   setObjectName("border");
}

//TrafficWidget::TrafficWidget(const TrafficWidget& orig)
//{
//}

TrafficWidget::~TrafficWidget()
{
}

void TrafficWidget::displayAC(Aircraft* ac)
{
   currentAC = ac;
   currentID = ac->getID();
   acLabel->setText(QString("Aircraft %1").arg(currentID));
   alt->setText(QString("%1 ft" ).arg(ac->getAlt(), 1, 'f', 0));
   spd->setText(QString("%1 kts").arg(ac->getSpd(), 1, 'f', 1));
   hdg->setText(QString("%1 deg").arg(ac->getHdg(), 1, 'f', 1));
   rng->setText(QString("%1 nm" ).arg(ac->getRng(), 1, 'f', 1));
   ber->setText(QString("%1 deg").arg(ac->getBer(), 1, 'f', 1));
   emit displayedACChanged(currentID);
}

void TrafficWidget::displayAC(int acID)
{
   if (acMap->contains(acID)) {
      displayAC(acMap->value(acID));
   }
}

void TrafficWidget::refreshAC(int acID)
{
   if (acMap->contains(acID)) {
      // Block the signals temporarily, since this is already the current AC
      blockSignals(true);
      displayAC(acMap->value(acID));
      blockSignals(false);
   }
}

void TrafficWidget::showNextAC()
{
   currentID++;
   if (!acMap->contains(currentID)) {
      currentID = 1;
   }
   displayAC(currentID);
}

void TrafficWidget::showPrevAC()
{
   currentID--;
   if (!acMap->contains(currentID) || currentID == 0) {
      currentID = acMap->size()-1;
   }
   displayAC(currentID);
}

void TrafficWidget::setupTrafficControls()
{
   QVBoxLayout* layout = new QVBoxLayout(this);
   QHBoxLayout* buttonLayout = new QHBoxLayout();
//   QGridLayout* layout = new QGridLayout(this);
//   QGridLayout* layoutAC= new QGridLayout(); // Selected AC label
//   QFormLayout* layoutL = new QFormLayout(); // left form
   QFormLayout* layoutC = new QFormLayout(); // center form
//   QFormLayout* layoutR = new QFormLayout(); // right form

   acLabel = new QLabel("Aircraft 1");
   acLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
   acLabel->setObjectName("border-light");

   nextButton = new QPushButton("Next");
   nextButton->setMinimumSize(QSize(30, 24));
   prevButton = new QPushButton("Prev");
   prevButton->setMinimumSize(QSize(30, 24));
   connect(nextButton, &QPushButton::released, this, &TrafficWidget::showNextAC);
   connect(prevButton, &QPushButton::released, this, &TrafficWidget::showPrevAC);

   buttonLayout->addWidget(prevButton);
   buttonLayout->addWidget(nextButton);
//   layoutAC->addWidget(prevButton, 0, 0);
//   layoutAC->addWidget(acLabel,    0, 2);
//   layoutAC->addWidget(nextButton, 0, 4);

   alt = new QLabel("0 ft");
   spd = new QLabel("0 kts");
   hdg = new QLabel("0 deg");
   typ = new QLabel("0");
   rng = new QLabel("0 nm");
   ber = new QLabel("0 deg");

   layoutC->addRow(tr("TYPE:"), typ);
   layoutC->addRow(tr("ALT:"),  alt);
   layoutC->addRow(tr("SPD:"),  spd);
   layoutC->addRow(tr("HDG:"),  hdg);
   layoutC->addRow(tr("RNG:"),  rng);
   layoutC->addRow(tr("BER:"),  ber);

   layout->addWidget(acLabel);
   layout->addLayout(layoutC);
   layout->addLayout(buttonLayout);

   
//   layout->addLayout(layoutL, 0, 0);
//   layout->addLayout(layoutC, 0, 2);
//   layout->addLayout(layoutR, 0, 4);
//   layout->addLayout(layoutAC,1, 0, 1, 6);
}
