/* 
 * File:   commscontroller.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 23, 2015, 12:33 PM
 */

#include "commscontroller.h"

#include <QDebug>

#include "core/cpdsettings.h"
#include "core/xplanedref.h"
#include "commswidget.h"

CommsController::CommsController(CPDSettings* _cpdSettings, QObject* _parent)
: QObject(_parent),
  cpdSettings(_cpdSettings)
{
}

//CommsController::CommsController(const CommsController& orig)
//{
//}

CommsController::~CommsController()
{
}

void CommsController::setTimes(float zulu, float local, float mission, float timer)
{
   timeGMT = zulu;
   timeLocal = local;
   timeMission = mission;
   timeTimer = timer;
   emit timesUpdated(timeGMT, timeLocal, timeMission, timeTimer);
}

void CommsController::setCom1(float freq, float standby)
{
   com1active = freq;
   com1standby = standby;
   emit comUpdate(COM1, freq, standby);
}

void CommsController::setCom2(float freq, float standby)
{
   com2active = freq;
   com2standby = standby;
   emit comUpdate(COM2, freq, standby);
}

void CommsController::setNav1(float freq, float standby)
{
   nav1active = freq;
   nav1standby = standby;
   emit comUpdate(NAV1, freq, standby);
}

void CommsController::setNav2(float freq, float standby)
{
   nav2active = freq;
   nav2standby = standby;
   emit comUpdate(NAV2, freq, standby);
}

void CommsController::comSwapped(CommType ct)
{
   if (ct & COM1) {
      com1Swapped();
   }
   else if (ct & COM2) {
      com2Swapped();
   }
   else if (ct & NAV1) {
      nav1Swapped();
   }
   else if (ct & NAV2) {
      nav2Swapped();
   }
}

// TODO: notify xplane of the swap
void CommsController::com1Swapped()
{
   float com1act  = com1active;
   notifyXPlaneComms(COM1_ACTIVE,  com1standby);
   notifyXPlaneComms(COM1_STANDBY, com1act);
}

void CommsController::com2Swapped()
{
   float com2act  = com2active;
   notifyXPlaneComms(COM2_ACTIVE,  com2standby);
   notifyXPlaneComms(COM2_STANDBY, com2act);
}

void CommsController::nav1Swapped()
{
   float nav1act  = nav1active;
   notifyXPlaneComms(NAV1_ACTIVE,  nav1standby);
   notifyXPlaneComms(NAV1_STANDBY, nav1act);
}

void CommsController::nav2Swapped()
{
   float nav2act  = nav2active;
   notifyXPlaneComms(NAV2_ACTIVE,  nav2standby);
   notifyXPlaneComms(NAV2_STANDBY, nav2act);
}

void CommsController::notifyXPlaneComms(CommType ct, float value)
{
   QString drefStr;
   if (ct == COM1_ACTIVE) {
      drefStr = QString(XPDR_RADIO_COM1_FREQ);
      com1active = value;
   }
   else if (ct == COM1_STANDBY) {
      drefStr = QString(XPDR_RADIO_COM1_STDBY);
      com1standby = value;
   }
   else if (ct == COM2_ACTIVE) {
      drefStr = QString(XPDR_RADIO_COM2_FREQ);
      com2active = value;
   }
   else if (ct == COM2_STANDBY) {
      drefStr = QString(XPDR_RADIO_COM2_STDBY);
      com2standby = value;
   }
   else if (ct == NAV1_ACTIVE) {
      drefStr = QString(XPDR_RADIO_NAV1_FREQ);
      nav1active = value;
   }
   else if (ct == NAV1_STANDBY) {
      drefStr = QString(XPDR_RADIO_NAV1_STDBY);
      nav1standby = value;
   }
   else if (ct == NAV2_ACTIVE) {
      drefStr = QString(XPDR_RADIO_NAV2_FREQ);
      nav2active = value;
   }
   else if (ct == NAV2_STANDBY) {
      drefStr = QString(XPDR_RADIO_NAV2_STDBY);
      nav2standby = value;
   }
   emit updateXPlaneComms(drefStr, (xpflt) value);
}

void CommsController::notifyXPlaneTimer(bool running)
{
   QString drefStr(XPDR_CP_TIMER_RUNNING);
   emit updateXPlaneTimer(drefStr, (int) running);
}

void CommsController::notifyXPlaneTimerReset()
{
   // Only have to send the reset, because the start/stop button has already
   // been toggled, in timeWidget.
   QString drefStr(XPDR_CP_TIMER_ELAPSED);
   emit updateXPlaneTimer(drefStr, 0.0);
}
