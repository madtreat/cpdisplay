/* 
 * File:   commswidget.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 15, 2015, 2:36 PM
 */

#ifndef TIMEWIDGET_H_
#define TIMEWIDGET_H_

#include <QFrame>
#include <QString>

class QLabel;
class QPushButton;

class CPDSettings;
class CommsController;


class TimeWidget : public QFrame {
   Q_OBJECT;

   enum TimeType {
      TIME_ZULU,
      TIME_LOCAL,
      TIME_MISSION,
      TIME_TIMER
   };

public:
   TimeWidget(CPDSettings* _cpdSettings, CommsController* _comC, QFrame* _parent=0);
   ~TimeWidget();

public slots:
   void setZuluTime(float time);
   void setLocalTime(float time);
   void setMissionTime(float time);
   void setTimerTime(float time);
   void setTimes(float zulu, float local, float mission, float timer);

   void toggleTimer(bool checked);
   void resetTimer();

private:
   CPDSettings* settings;
   CommsController* comC;

   QLabel* zuluTime;
   QLabel* localTime;
   QLabel* missionTime;
   QLabel* timerTime;

   QPushButton* timerStartStop;
   QPushButton* timerReset;

   bool justInitialized; // to make sure timer start/stop button's state is accurate

   QString formatTime(float time, TimeType type=TIME_ZULU);
};



#endif // TIMEWIDGET_H_