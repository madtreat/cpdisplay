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

class ThrottleWidget : public QWidget {
   Q_OBJECT;
   
public:
   ThrottleWidget(int _engNum, int _max, int _value, QWidget* _parent=0);
   ThrottleWidget(const ThrottleWidget& orig) = delete;
   virtual ~ThrottleWidget();
   
public slots:
   void setValue(int _value);
   
private:
   int engineNum;
   int maxVal;
   int value;
   
   QVBoxLayout* layout;
   QLabel* label;
   QProgressBar* throttle;
};

typedef QList<ThrottleWidget*> ThrottleList;

#endif	/* THROTTLEWIDGET_H */

