/* 
 * File:   commscontroller.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 23, 2015, 12:33 PM
 */

#ifndef COMMSCONTROLLER_H
#define COMMSCONTROLLER_H

#include <QObject>

#include "comms.h"
//#include "commswidget.h"

class HDDSettings;
class CommsWidget;


class CommsController : public QObject {
   Q_OBJECT;

public:
   CommsController(HDDSettings* _hddSettings, QObject* _parent = 0);
   CommsController(const CommsController& orig) = delete;
   virtual ~CommsController();
   
//   CommsWidget*  getWidget()  const { return commsWidget; }

public slots:
   void setTimes(float _zulu, float _local);
   void setCom1(float freq, float standby);
   void setCom2(float freq, float standby);
   void setNav1(float freq, float standby);
   void setNav2(float freq, float standby);
   
   void comSwapped(CommType ct);
   void com1Swapped();
   void com2Swapped();
   void nav1Swapped();
   void nav2Swapped();
   
signals:
   void comUpdate(CommType ct, float active, float standby);

private:
   HDDSettings* hddSettings;
//   CommsWidget* commsWidget;
   
   // Comms frequencies
   float com1active;
   float com1standby;
   float com2active;
   float com2standby;
   float nav1active;
   float nav1standby;
   float nav2active;
   float nav2standby;
   
   float timeGMT;
   float timeLocal;
};

#endif	/* COMMSCONTROLLER_H */

