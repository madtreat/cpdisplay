/* 
 * File:   trafficwidget.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 15, 2015, 2:36 PM
 */

#include "trafficwidget.h"

#include <QFormLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

#include "core/aircraft.h"
#include "core/hddsettings.h"

TrafficWidget::TrafficWidget(HDDSettings* _hddSettings, ACMap* _acMap, QWidget* _parent)
: QWidget(_parent),
  hddSettings(_hddSettings),
  acMap(_acMap),
  currentID(1)
{
   if (acMap->contains(currentID)) {
      currentAC = acMap->value(currentID);
   }
   else {
      currentAC = NULL;
   }
   setupTrafficControls();
   
//   setMinimumSize(QSize(320, 320));
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
   acLabel->setText(QString("Aircraft %1").arg(ac->getID()));
   alt->setText(QString("%1").arg(ac->getAlt(), 1, 'f', 1));
   spd->setText(QString("%1").arg(ac->getSpd(), 1, 'f', 1));
   hdg->setText(QString("%1").arg(ac->getHdg(), 1, 'f', 1));
   rng->setText(QString("%1").arg(ac->getRng(), 1, 'f', 1));
   ber->setText(QString("%1").arg(ac->getBer(), 1, 'f', 1));
   emit displayedACChanged(currentID);
}

void TrafficWidget::displayAC(int acID)
{
   if (acMap->contains(acID)) {
      displayAC(acMap->value(acID));
   }
   else {
      // do nothing?
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
   QGridLayout* layout = new QGridLayout(this);
   QGridLayout* layoutAC= new QGridLayout(); // Selected AC label
   QFormLayout* layoutL = new QFormLayout(); // left form
   QFormLayout* layoutC = new QFormLayout(); // center form
   QFormLayout* layoutR = new QFormLayout(); // right form
   
   acLabel = new QLabel("Aircraft 1");
   acLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
   nextButton = new QPushButton("Next AC");
   prevButton = new QPushButton("Prev AC");
   connect(nextButton, &QPushButton::released, this, &TrafficWidget::showNextAC);
   connect(prevButton, &QPushButton::released, this, &TrafficWidget::showPrevAC);
   
   layoutAC->addWidget(prevButton, 0, 0);
   layoutAC->addWidget(acLabel,    0, 2);
   layoutAC->addWidget(nextButton, 0, 4);
   
   alt = new QLabel("0");
   spd = new QLabel("0");
   hdg = new QLabel("0");
   typ = new QLabel("0");
   rng = new QLabel("0");
   ber = new QLabel("0");
   
   layoutL->addRow(tr("ALT:"),  alt);
   layoutL->addRow(tr("SPD:"),  spd);
   layoutC->addRow(tr("HDG:"),  hdg);
   layoutC->addRow(tr("TYPE:"), typ);
   layoutR->addRow(tr("RNG:"),  rng);
   layoutR->addRow(tr("BER:"),  ber);
   
   layout->addLayout(layoutL, 0, 0);
   layout->addLayout(layoutC, 0, 2);
   layout->addLayout(layoutR, 0, 4);
   layout->addLayout(layoutAC,1, 0, 1, 6);
}