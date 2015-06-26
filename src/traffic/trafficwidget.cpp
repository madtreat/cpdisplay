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

#include "core/aircraft.h"
#include "core/hddsettings.h"

TrafficWidget::TrafficWidget(HDDSettings* _hddSettings, QWidget* _parent)
: QWidget(_parent),
  hddSettings(_hddSettings)
{
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
   alt->setText(QString("%1").arg(ac->getAlt()));
   spd->setText(QString("%1").arg(ac->getSpd()));
   hdg->setText(QString("%1").arg(ac->getHdg()));
   rng->setText(QString("%1").arg(ac->getRng()));
   ber->setText(QString("%1").arg(ac->getBer()));
}

void TrafficWidget::setupTrafficControls()
{
   QGridLayout* layout = new QGridLayout(this);
   QFormLayout* layoutL = new QFormLayout(); // left form
   QFormLayout* layoutC = new QFormLayout(); // center form
   QFormLayout* layoutR = new QFormLayout(); // right form
   
   alt = new QLabel("0");
   spd = new QLabel("0");
   hdg = new QLabel("0");
   typ = new QLabel("0");
   rng = new QLabel("0");
   ber = new QLabel("0");
   
   layoutL->addRow(tr("ALT"),  alt);
   layoutL->addRow(tr("SPD"),  spd);
   layoutC->addRow(tr("HDG"),  hdg);
   layoutC->addRow(tr("TYPE"), typ);
   layoutR->addRow(tr("RNG"),  rng);
   layoutR->addRow(tr("BER"),  ber);
   
   layout->addLayout(layoutL, 0, 0);
   layout->addLayout(layoutC, 0, 1);
   layout->addLayout(layoutR, 0, 2);
}