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

CommsWidget::CommsWidget(
  CPDSettings* _cpdSettings,
  CommsController* _comC,
  QFrame* _parent
)
: QFrame(_parent),
//m_border(true),
cpdSettings(_cpdSettings),
comC(_comC) {
  setupCommsControls();

  setObjectName("border");
}

//CommsWidget::CommsWidget(const CommsWidget& orig) {
//}

CommsWidget::~CommsWidget() {
}

void CommsWidget::setupCommsControls() {
  QVBoxLayout* layout = new QVBoxLayout(this);
//  layout->setContentsMargins(0, 0, 0, 0);

  QLabel* commsLabel = new QLabel("Radios");
  commsLabel->setAlignment(Qt::AlignHCenter);
  commsLabel->setObjectName("border-light");

  QHBoxLayout* header = new QHBoxLayout();
  header->setContentsMargins(0, 0, 0, 0);

  QLabel* activeLabel = new QLabel("Active");
  QLabel* standbyLabel = new QLabel("Standby");
  header->addStretch(5);
  header->addWidget(activeLabel);
  header->addStretch(6);
  header->addWidget(standbyLabel);
  header->addStretch(2);

  com1 = new CommsLine(comC, COM1);
  com2 = new CommsLine(comC, COM2);
  nav1 = new CommsLine(comC, NAV1);
  nav2 = new CommsLine(comC, NAV2);

  layout->addWidget(commsLabel);
  layout->addLayout(header);
  layout->addWidget(com1);
  layout->addWidget(com2);
  layout->addWidget(nav1);
  layout->addWidget(nav2);
  layout->addStretch(2);
}
