/* 
 * File:   throttlewidget.h
 * Author: madtreat
 *
 * Created on July 10, 2015, 2:46 PM
 */

#ifndef THROTTLEWIDGET_H
#define THROTTLEWIDGET_H

#include <QWidget>
#include <QList>
#include <QVBoxLayout>

class QProgressBar;
class QLabel;

class EngineController;


class ThrottleWidget : public QWidget {
   Q_OBJECT;
   
public:
   ThrottleWidget(EngineController* _engC, int _engNum, int _max, int _value, QWidget* _parent=0);
   ThrottleWidget(const ThrottleWidget& orig) = delete;
   virtual ~ThrottleWidget();
   
public slots:
   void setValue(int _value, int _engNum);
   
private:
   EngineController* engC;
   int engineNum;
   int maxVal;
   int value;
   
   QVBoxLayout* layout;
   QProgressBar* throttle;
};

#endif	/* THROTTLEWIDGET_H */

