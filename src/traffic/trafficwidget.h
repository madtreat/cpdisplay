/* 
 * File:   trafficwidget.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 15, 2015, 2:37 PM
 */

#ifndef TRAFFICWIDGET_
#define TRAFFICWIDGET_

#include <QWidget>

#include "core/aircraft.h"

class QLabel;
class QPushButton;

class Aircraft;
class HDDSettings;


class TrafficWidget : public QWidget {
   Q_OBJECT;

public:
   TrafficWidget(HDDSettings* _hddSettings, ACMap* _acMap, QWidget* _parent = 0);
   TrafficWidget(const TrafficWidget& orig) = delete;
   virtual ~TrafficWidget();

public slots:
   void displayAC(Aircraft* ac);
   void displayAC(int acID);
   void showNextAC();
   void showPrevAC();
   
signals:
   void displayedACChanged(int id);

private:
   HDDSettings*   hddSettings;
   ACMap*         acMap;
   Aircraft*      currentAC;
   int            currentID;
   
   QLabel*        acLabel; // currently selected aircraft ID
   QPushButton*   nextButton;
   QPushButton*   prevButton;
   
   // The textual labels that will be connected to XPlane
   QLabel* alt; // altitude
   QLabel* spd; // speed
   QLabel* hdg; // heading
   QLabel* typ; // type
   QLabel* rng; // range
   QLabel* ber; // bearing

   void setupTrafficControls();
};

#endif	/* TRAFFICWIDGET_ */

