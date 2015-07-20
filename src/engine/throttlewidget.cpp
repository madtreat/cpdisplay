/* 
 * File:   throttlewidget.cpp
 * Author: madtreat
 * 
 * Created on July 10, 2015, 2:46 PM
 */

#include "throttlewidget.h"
#include <QProgressBar>
#include <QLabel>

ThrottleWidget::ThrottleWidget(int _engNum, int _max, int _value, QWidget* _parent)
: QWidget(_parent),
  engineNum(_engNum),
  maxVal(_max),
  value(_value)
{
   label = new QLabel(QString("ENG %1").arg(engineNum+1));
   //label->setMinimumWidth(50);
   label->setMaximumSize(QSize(40, 20));
   
   throttle = new QProgressBar();
   throttle->setOrientation(Qt::Vertical);
   throttle->setTextDirection(QProgressBar::BottomToTop);
//   throttle->setMinimumWidth(10);
   throttle->setMinimum(0);
   throttle->setMaximum(maxVal);
   throttle->setValue(value);
   throttle->setMaximumSize(QSize(30, 300));
   
   layout = new QVBoxLayout(this);
   layout->addWidget(label);//, 0, 0);
   layout->addWidget(throttle);//, 1, 0);
}

//ThrottleWidget::ThrottleWidget(const ThrottleWidget& orig)
//{
//}

ThrottleWidget::~ThrottleWidget()
{
}

void ThrottleWidget::setValue(int _value)
{
   value = _value;
   throttle->setValue(value);
}
