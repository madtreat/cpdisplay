/* 
 * File:   commscontroller.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 23, 2015, 12:33 PM
 */

#include "commscontroller.h"

#include <QDebug>

#include "core/cpdsettings.h"
#include "commswidget.h"

CommsController::CommsController(CPDSettings* _cpdSettings, QObject* _parent)
: QObject(_parent),
  cpdSettings(_cpdSettings)
{
//   commsWidget = new CommsWidget(cpdSettings);
}

//CommsController::CommsController(const CommsController& orig)
//{
//}

CommsController::~CommsController()
{
}

void CommsController::setTimes(float _zulu, float _local)
{
//   QString zuluStr = QString("%1").arg(_zulu);
//   QString localStr = QString("%1").arg(_local);
   timeGMT = _zulu;
   timeLocal = _local;
   // emit timesUpdated();
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
   com1active     = com1standby;
   com1standby    = com1act;
}

void CommsController::com2Swapped()
{
   float com2act  = com2active;
   com2active     = com2standby;
   com2standby    = com2act;
}

void CommsController::nav1Swapped()
{
   float nav1act  = nav1active;
   nav1active     = nav1standby;
   nav1standby    = nav1act;
}

void CommsController::nav2Swapped()
{
   float nav2act  = nav2active;
   nav2active     = nav2standby;
   nav2standby    = nav2act;
}
