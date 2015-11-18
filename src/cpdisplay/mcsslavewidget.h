/*
 * A single column representing the status of a single MCS Slave System.
 */

#ifndef MCSSLAVEWIDGET_H
#define MCSSLAVEWIDGET_H

#include <QFrame>
#include <QPushButton>
#include <QLabel>


struct SlaveSystem;
class CPDSettings;
class SwitchBoard;

class MCSSlaveWidget : public QFrame {
   Q_OBJECT;

public:
   MCSSlaveWidget(CPDSettings* _settings, SwitchBoard* _sb, int _slaveID, QFrame* _parent=0);
   ~MCSSlaveWidget();

public slots:
   void pauseSimulator(bool paused);  // button checked == sim paused
   void lowerGear(bool down);          // button checked == gear down
   void activateBreaks(bool active);   // button checked == breaks on

private:
   CPDSettings*   settings;
   SwitchBoard*   sb;
   int            slaveID;
   SlaveSystem*   slaveConfig;

   // Labels and data
   QLabel*        name;
   QPushButton*   pause;
   QLabel*        status;
   QLabel*        ipSim;      // IP Address
   QLabel*        ipCPD;      // IP Address
   QLabel*        aircraft;
   QLabel*        flaps;      // percent
   QPushButton*   gearStatus; // up/down
   QLabel*        altitude;   // FSL
   QLabel*        airspeed;   // KIAS
   QLabel*        mach;
   QLabel*        heading;
   QLabel*        fuel;       // percent
   QLabel*        timeLocal;
   QLabel*        timeZulu;
   QPushButton*   wheelBreaks;// on/off
};

typedef MCSSlaveWidget MCSSW;

#endif
