/* 
 * File:   commswidget.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 15, 2015, 2:36 PM
 */

#include "commswidget.h"

#include <QFormLayout>
#include <QGridLayout>
#include <QLabel>

#include "core/hddsettings.h"

CommsWidget::CommsWidget(HDDSettings* _hddSettings, QWidget* _parent)
: QWidget(_parent),
  hddSettings(_hddSettings)
{
   setupCommsControls();
   
//   setMinimumSize(QSize(140, 210));
   setContentsMargins(0, 0, 0, 0);
}

//CommsWidget::CommsWidget(const CommsWidget& orig)
//{
//}

CommsWidget::~CommsWidget()
{
}

void CommsWidget::setTimes(float _zulu, float _local)
{
   QString zuluStr = QString("%1").arg(_zulu);
   QString localStr = QString("%1").arg(_local);
   gmt->setText(zuluStr);
   local->setText(localStr);
}

void CommsWidget::setCom1(float freq, float standby)
{
   com1->setText(QString("%1").arg(freq));
}

void CommsWidget::setCom2(float freq, float standby)
{
   com2->setText(QString("%1").arg(freq));
}

void CommsWidget::setNav1(float freq, float standby)
{
   nav1->setText(QString("%1").arg(freq));
}

void CommsWidget::setNav2(float freq, float standby)
{
   nav2->setText(QString("%1").arg(freq));
}

void CommsWidget::setupCommsControls()
{
   QGridLayout* layout  = new QGridLayout(this); // overall layout
   QFormLayout* layoutL = new QFormLayout(); // left form
   QFormLayout* layoutC = new QFormLayout(); // center form
   QFormLayout* layoutR = new QFormLayout(); // right form
   
   com1 = new QLabel("0.00");
   com2 = new QLabel("0.00");
   nav1 = new QLabel("0.00");
   nav2 = new QLabel("0.00");
   gmt  = new QLabel("0:00:00");
   local= new QLabel("0:00:00");
   
   layoutL->addRow(tr("COM 1:"), com1);
   layoutL->addRow(tr("COM 2:"), com2);
   layoutC->addRow(tr("NAV 1:"), nav1);
   layoutC->addRow(tr("NAV 2:"), nav2);
   layoutR->addRow(tr("GMT:"), gmt);
   layoutR->addRow(tr("local:"), local);
   
   layout->addLayout(layoutL, 0, 0);
   layout->addLayout(layoutC, 0, 1);
   layout->addLayout(layoutR, 0, 2);
}