#ifndef MCSWIDGET_H
#define MCSWIDGET_H

#include <QFrame>
#include <QLabel>
#include <QPushButton>


class SwitchBoard;
class CPDSettings;
class MCSSlaveWidget;

typedef QList<MCSSlaveWidget*> SlaveList;

class MCSWidget : public QFrame {
   Q_OBJECT;

public:
   MCSWidget(CPDSettings* _settings, QList<SwitchBoard*> _sbs, QFrame* _parent=0);
   ~MCSWidget();

public slots:
   void pauseAllSimulators(bool paused);  // button checked == paused
   void lowerAllGear(bool down);          // button checked == gear down
   void activateAllBreaks(bool active);   // button checked == breaks on

private:
   CPDSettings*         settings;
   SlaveList            slaveWidgets;
   QList<SwitchBoard*>  slaveSwitches;

   // Labels and data
   QLabel*        name;
   QLabel*        tail;
   QLabel*        status;
   QLabel*        ipSim;      // IP Address
   QLabel*        ipCPD;      // IP Address
   QLabel*        aircraft;
   QLabel*        flaps;      // percent
   QLabel*        altitude;   // FSL
   QLabel*        airspeed;   // KIAS
   QLabel*        mach;
   QLabel*        heading;
   QLabel*        fuel;       // percent
   QLabel*        timeLocal;
   QLabel*        timeZulu;
   QPushButton*   pause;
   QPushButton*   gearStatus; // up/down
   QPushButton*   wheelBreaks;// on/off

   QLayout* createSlaveWidgetLabels();
};

#endif
