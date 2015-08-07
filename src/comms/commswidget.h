/* 
 * File:   commswidget.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 15, 2015, 2:37 PM
 */

#ifndef COMMSWIDGET_
#define COMMSWIDGET_

#include <QFrame>
#include <QString>

#include "comms.h"

class QLabel;

class CPDSettings;
class CommsController;
class CommsLine;


class CommsWidget : public QFrame {
   Q_OBJECT;

public:
   CommsWidget(CPDSettings* _cpdSettings, CommsController* _comC, QFrame* _parent = 0);
   CommsWidget(const CommsWidget& orig) = delete;
   virtual ~CommsWidget();
   
public slots:
   void setTimes(float _zulu, float _local);

private:
   CPDSettings* cpdSettings;
   CommsController* comC;
   
   // The textual labels that will be connected to XPlane
   CommsLine* com1;
   CommsLine* com2;
   CommsLine* nav1;
   CommsLine* nav2;
   
//   QLabel* gmt;
//   QLabel* local;

   void setupCommsControls();
};

#endif	/* COMMSWIDGET_ */

