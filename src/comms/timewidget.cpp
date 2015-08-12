/* 
 * File:   timewidget.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on August 12, 12:57 PM
 */

#include "timewidget.h"

#include <QFormLayout>
#include <QLabel>

#include "commscontroller.h"


TimeWidget::TimeWidget(CPDSettings* _cpdSettings, CommsController* _comC, QFrame* _parent)
: QFrame(_parent),
  settings(_cpdSettings),
  comC(_comC)
{
   connect(comC, &COMC::timesUpdated, this, &TimeWidget::setTimes);
   QFormLayout* layout = new QFormLayout(this);

   QString timeBase = "00:00:00";

   zuluTime    = new QLabel(timeBase);
   localTime   = new QLabel(timeBase);
   missionTime = new QLabel(timeBase);
   timerTime   = new QLabel(timeBase);

  layout->addRow(tr("GMT:"), zuluTime);
  layout->addRow(tr("Local:"), localTime);
  layout->addRow(tr("Mission:"), missionTime);
  layout->addRow(tr("Timer:"), timerTime);

   setObjectName("border");
}

TimeWidget::~TimeWidget()
{

}

QString TimeWidget::formatTime(float time, TimeType type)
{
   int hours = (int) time;//hoursPart;
   double minRemainder = (time - hours) * 60;
   int minutes = (int) minRemainder;
   double secRemainder = (minRemainder - minutes) * 60;
   int seconds = (int) secRemainder;

   QString str = QString("%1:%2:%3")
                  .arg(hours  , 2, 10, QChar('0'))
                  .arg(minutes, 2, 10, QChar('0'))
                  .arg(seconds, 2, 10, QChar('0'));
   // qDebug() << "Time (raw | hms):" << time << "|" << str;
   return str;
}

void TimeWidget::setZuluTime(float time)
{
   zuluTime->setText(formatTime(time, TIME_ZULU));
}

void TimeWidget::setLocalTime(float time)
{
   localTime->setText(formatTime(time, TIME_LOCAL));
}

void TimeWidget::setMissionTime(float time)
{
   missionTime->setText(formatTime(time, TIME_MISSION));
}

void TimeWidget::setTimerTime(float time)
{
   timerTime->setText(formatTime(time, TIME_TIMER));
}

void TimeWidget::setTimes(float zulu, float local, float mission, float timer)
{
   setZuluTime(zulu);
   setLocalTime(local);
   setMissionTime(mission);
   setTimerTime(timer);
}
