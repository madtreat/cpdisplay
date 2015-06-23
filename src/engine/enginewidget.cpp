/* 
 * File:   enginewidget.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 15, 2015, 2:36 PM
 */

#include "enginewidget.h"

#include <QGridLayout>

#include "core/hddsettings.h"

EngineWidget::EngineWidget(HDDSettings* _hddSettings, QWidget* _parent)
: QWidget(_parent),
  hddSettings(_hddSettings)
{
   setupEngineControls();
   
   setMinimumSize(QSize(320, 320));
}

//EngineWidget::EngineWidget(const EngineWidget& orig)
//{
//}

EngineWidget::~EngineWidget()
{
}

void EngineWidget::setupEngineControls()
{
   QGridLayout* layout = new QGridLayout(this);
   
   
}