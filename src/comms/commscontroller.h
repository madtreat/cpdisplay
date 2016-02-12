/* 
 * File:   commscontroller.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 23, 2015, 12:33 PM
 */

#ifndef COMMSCONTROLLER_H
#define COMMSCONTROLLER_H

#include <QObject>

#include "core/xplanedata.h"
#include "comms.h"

class CPDSettings;


class CommsController : public QObject {
  Q_OBJECT;

public:
  CommsController(CPDSettings* _cpdSettings, QObject* _parent = 0);
  CommsController(const CommsController& orig) = delete;
  virtual ~CommsController();

public slots:
  void setTimes(float zulu, float local, float mission, float timer);
  void setCom1(float freq, float standby);
  void setCom2(float freq, float standby);
  void setNav1(float freq, float standby);
  void setNav2(float freq, float standby);
   
  void comSwapped(CommType ct);
  void com1Swapped();
  void com2Swapped();
  void nav1Swapped();
  void nav2Swapped();
  void notifyXPlaneComms(CommType ct, float value);

  void notifyXPlaneTimer(bool running);
  void notifyXPlaneTimerReset();
   
signals:
  void timesUpdated(float zulu, float local, float mission, float timer);
  void comUpdate(CommType ct, float active, float standby);
  void updateXPlaneComms(QString drefStr, xpflt value);
  void updateXPlaneTimer(QString drefStr, xpint value);

private:
  CPDSettings* cpdSettings;
   
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
  float timeMission;
  float timeTimer;
};

typedef CommsController COMC;

#endif	/* COMMSCONTROLLER_H */

