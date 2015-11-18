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

private:
   CPDSettings*         settings;
   SlaveList            slaveWidgets;
   QList<SwitchBoard*>  slaveSwitches;

   // Labels and data
   QPushButton*   pause;
   QLabel*        name;
   QLabel*        status;
   QLabel*        ipSim;      // IP Address
   QLabel*        ipCPD;      // IP Address
   QLabel*        aircraft;
   QLabel*        flaps;      // percent
   QLabel*        gearStatus; // up/down
   QLabel*        altitude;   // FSL
   QLabel*        airspeed;   // KIAS
   QLabel*        mach;
   QLabel*        heading;
   QLabel*        fuel;       // percent
   QLabel*        timeLocal;
   QLabel*        timeZulu;
   QLabel*        wheelBreaks;// on/off

   QLayout* createSlaveWidgetLabels();
};

#endif
