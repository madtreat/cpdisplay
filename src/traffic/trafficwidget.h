/* 
 * File:   trafficwidget.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 15, 2015, 2:37 PM
 */

#ifndef TRAFFICWIDGET_
#define TRAFFICWIDGET_

#include <QWidget>

class QLabel;

class Aircraft;
class HDDSettings;


class TrafficWidget : public QWidget {
   Q_OBJECT;

public:
   TrafficWidget(HDDSettings* _hddSettings, QWidget* _parent = 0);
   TrafficWidget(const TrafficWidget& orig) = delete;
   virtual ~TrafficWidget();

public slots:
   void displayAC(Aircraft* ac);

private:
   HDDSettings* hddSettings;
   Aircraft*    currentAC;
   
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

