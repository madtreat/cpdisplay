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
#include "singleenginewidget.h"


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
   for (int i = 0; i < numEngines; i++) {
      SingleEngineWidget* eng = new SingleEngineWidget(hddSettings, engC, i);
      engines.append(eng);
      layout->addWidget(eng);
   }
   
   layout->addStretch(2);
}
