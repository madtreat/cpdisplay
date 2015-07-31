/* 
 * File:   enginewidget.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 15, 2015, 2:36 PM
 */

#include "enginewidget.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

#include "core/hddsettings.h"
#include "enginecontroller.h"
#include "enginedialwidget.h"


EngineWidget::EngineWidget(HDDSettings* _hddSettings, EngineController* _engC, int _numEngines, QFrame* _parent)
: QFrame(_parent),
  hddSettings(_hddSettings),
  engC(_engC),
  numEngines(_numEngines)
{
   setupEngineControls();
   
   setMinimumSize(QSize(65, 180));
   setObjectName("border");
}

//EngineWidget::EngineWidget(const EngineWidget& orig)
//{
//}

EngineWidget::~EngineWidget()
{
}

void EngineWidget::setupEngineControls()
{
   QHBoxLayout* layout = new QHBoxLayout(this);
   layout->setContentsMargins(0, 0, 0, 0);
   
   // This loop will create throttles in pairs, to keep them grouped better on the main layout
   for (int i = 0; i < numEngines/2; i++) {
      QHBoxLayout* pairLayout = new QHBoxLayout();
      ThrottleWidget* lt = new ThrottleWidget(engC, i*2, 100, 0); // left throttle of pair
      pairLayout->addWidget(lt);
      throttles.append(lt);
      
      // If numEngines is even, add a second throttle to this pair
      if (numEngines % 2 == 0) {
         ThrottleWidget* rt = new ThrottleWidget(engC, (i*2)+1, 100, 0); // right throttle of pair
         pairLayout->addWidget(rt);
         throttles.append(rt);
      }
      
      layout->addLayout(pairLayout);
   }
   
   // Add the oil widgets
   oilTWidget = new EngineDialWidget(engC, OIL_TEMP, 20, 180, 0);
   oilPWidget = new EngineDialWidget(engC, OIL_PRESSURE, 40, 120, 0);
   
   QVBoxLayout* oilLayout = new QVBoxLayout();
   oilLayout->setContentsMargins(0, 0, 0, 0);
   oilLayout->addWidget(oilTWidget);
   oilLayout->addWidget(oilPWidget);
   layout->addLayout(oilLayout);
   
   layout->addStretch(2);
}
