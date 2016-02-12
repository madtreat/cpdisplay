/* 
 * File:   trafficwidget.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 15, 2015, 2:37 PM
 */

#ifndef TRAFFICWIDGET_
#define TRAFFICWIDGET_

#include <QFrame>

#include "core/aircraft.h"

class QLabel;
class QPushButton;

class Aircraft;
class CPDSettings;
class TrafficController;


class TrafficWidget : public QFrame {
  Q_OBJECT;

public:
  TrafficWidget(CPDSettings* _cpdSettings, TrafficController* _tfcC, ACMap* _acMap, QFrame* _parent = 0);
  TrafficWidget(const TrafficWidget& orig) = delete;
  virtual ~TrafficWidget();

public slots:
  void displayAC(Aircraft* ac);
  void displayAC(int acID);
  void refreshAC(int acID);
  void showNextAC();
  void showPrevAC();

signals:
  void displayedACChanged(int id);

private:
  CPDSettings*   cpdSettings;

  TrafficController* tfcC;
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

#endif // TRAFFICWIDGET_

