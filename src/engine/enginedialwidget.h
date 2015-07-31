/* 
 * File:   oilwidget.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on July 31, 2015, 11:02 AM
 */

#ifndef OILWIDGET_H
#define OILWIDGET_H

#include <QWidget>

#include "engineconsts.h"

class QLabel;
//class QDial;

class EngineController;
class EngineDial;


class EngineDialWidget : public QWidget {
   Q_OBJECT;

public:
   EngineDialWidget(EngineController* _engC, EngineDialType _type, int _min, int _max, int _value, QWidget* _parent = 0);
   EngineDialWidget(const EngineDialWidget& orig) = delete;
   virtual ~EngineDialWidget();

public slots:
   void setValue(float _value);

private:
   EngineController* engC;
   EngineDialType     type;
   
   int      valueMin;
   int      valueMax;
   int      value;
   
   QString     labelBase;
   QLabel*     label;
//   QDial*      dial;
//   SkinnedDial*   dial;
   EngineDial* dial;

};

#endif	/* OILWIDGET_H */

