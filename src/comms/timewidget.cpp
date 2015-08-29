/* 
 * File:   timewidget.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on August 12, 12:57 PM
 */

#include "timewidget.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QWidget>

#include "commscontroller.h"


TimeWidget::TimeWidget(CPDSettings* _cpdSettings, CommsController* _comC, QFrame* _parent)
: QFrame(_parent),
  settings(_cpdSettings),
  comC(_comC),
  justInitialized(true)
{
   connect(comC, &COMC::timesUpdated, this, &TimeWidget::setTimes);

   QHBoxLayout* layout = new QHBoxLayout(this);
   QFormLayout* form = new QFormLayout();
   form->setContentsMargins(0, 0, 0, 0);

   // Set up the time form layout
   QString timeBase = "00:00:00";
   zuluTime    = new QLabel(timeBase);
   localTime   = new QLabel(timeBase);
   missionTime = new QLabel(timeBase);
   timerTime   = new QLabel(timeBase);

   form->addRow(tr("GMT:"), zuluTime);
   form->addRow(tr("Local:"), localTime);
   form->addRow(tr("Mission:"), missionTime);
   form->addRow(tr("Timer:"), timerTime);

   // Set up the timer buttons
   QWidget* timerButtons = new QWidget();
   timerButtons->setObjectName("border-light");
   QVBoxLayout* timerLayout = new QVBoxLayout();//timerButtons);
   timerLayout->setContentsMargins(0, 0, 0, 0);

   QLabel* timerLabel = new QLabel("Timer");
   timerLabel->setObjectName("border-light");
   timerLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
   timerStartStop = new QPushButton("Timer Start");
   timerStartStop->setCheckable(true);
   // timerStartStop->setChecked(false); // stop the timer at init
   timerStartStop->setMinimumHeight(27);
   timerReset = new QPushButton("Timer Reset");
   timerReset->setMinimumHeight(27);

   connect(timerStartStop, &QPushButton::toggled, this, &TimeWidget::toggleTimer);
   connect(timerReset,     &QPushButton::clicked, this, &TimeWidget::resetTimer);

   // timerLayout->addWidget(timerLabel);
   timerLayout->addWidget(timerStartStop);
   timerLayout->addWidget(timerReset);
   // timerLayout->addStretch();

   // Set up the main layout
   layout->addLayout(form);
   // layout->addWidget(timerButtons);
   layout->addLayout(timerLayout);

   setObjectName("border");
   justInitialized = false;
}

TimeWidget::~TimeWidget()
{

}

QString TimeWidget::formatTime(float time, TimeType type)
{
   QString str;
   if (type == TIME_ZULU || type == TIME_LOCAL) {
      int hours = (int) time;
      double minRemainder = (time - hours) * 60;
      int minutes = (int) minRemainder;
      double secRemainder = (minRemainder - minutes) * 60;
      int seconds = (int) secRemainder;
      str = QString("%1:%2:%3")
                     .arg(hours  , 2, 10, QChar('0'))
                     .arg(minutes, 2, 10, QChar('0'))
                     .arg(seconds, 2, 10, QChar('0'));
   }
   else if (type == TIME_MISSION || type == TIME_TIMER) {
      int intPart = (int) time;
      int minutes = (int) time / 60;
      int seconds = (int) time % 60;
      double fracRemainder = (time - intPart) * 100;
      int frac = (int) fracRemainder;
      str = QString("%1:%2.%3")
                     .arg(minutes, 2, 10, QChar('0'))
                     .arg(seconds, 2, 10, QChar('0'))
                     .arg(frac   , 2, 10, QChar('0'));
   }
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
   // If the app just started and the timer was already running, make sure
   // it is reflected in the button state.
   if (justInitialized && time > 0) { //&& !timerStartStop->isChecked()) {
      timerStartStop->blockSignals(true);
      timerStartStop->setChecked(true);
      timerStartStop->blockSignals(false);
   }
}

void TimeWidget::setTimes(float zulu, float local, float mission, float timer)
{
   setZuluTime(zulu);
   setLocalTime(local);
   setMissionTime(mission);
   setTimerTime(timer);
}

// In this context, checked == timer is running
void TimeWidget::toggleTimer(bool checked)
{
   timerStartStop->setText(checked ? "Stop" : "Start");
   // notify xplane
   comC->notifyXPlaneTimer(checked);
}

void TimeWidget::resetTimer()
{
   timerStartStop->setChecked(false);
   comC->notifyXPlaneTimerReset();
}
