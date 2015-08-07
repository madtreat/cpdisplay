/* 
 * File:   commswidget.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 15, 2015, 2:36 PM
 */

#include "commswidget.h"

#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

#include "core/cpdsettings.h"
#include "commscontroller.h"
#include "commsline.h"

CommsWidget::CommsWidget(CPDSettings* _cpdSettings, CommsController* _comC, QFrame* _parent)
: QFrame(_parent),
//  m_border(true),
  cpdSettings(_cpdSettings),
  comC(_comC)
{
   setupCommsControls();
   
   setContentsMargins(0, 0, 0, 0);
   setObjectName("border");
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
//   gmt->setText(zuluStr);
//   local->setText(localStr);
}

void CommsWidget::setupCommsControls()
{
   QVBoxLayout* layout = new QVBoxLayout(this);
   layout->setContentsMargins(0, 0, 0, 0);
   QHBoxLayout* header = new QHBoxLayout();
   
   QLabel* activeLabel = new QLabel("Active");
   QLabel* standbyLabel = new QLabel("Standby");
   header->addStretch(2);
   header->addWidget(activeLabel);
   header->addStretch(3);
   header->addWidget(standbyLabel);
   header->addStretch(1);
   
   com1 = new CommsLine(comC, COM1);
   com2 = new CommsLine(comC, COM2);
   nav1 = new CommsLine(comC, NAV1);
   nav2 = new CommsLine(comC, NAV2);
   
   layout->addLayout(header);
   layout->addWidget(com1);
   layout->addWidget(com2);
   layout->addWidget(nav1);
   layout->addWidget(nav2);
   
//   QFormLayout* layoutR = new QFormLayout(); // right form
//   
//   gmt  = new QLabel("0:00:00");
//   local= new QLabel("0:00:00");
//   layoutR->addRow(tr("GMT:"), gmt);
//   layoutR->addRow(tr("local:"), local);
}
