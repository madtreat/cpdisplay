#ifndef CPDWIDGET_H
#define CPDWIDGET_H

#include <QFrame>

#include "core/aircraft.h"
#include "map/mapcontroller.h"

class QFrame;
class QToolBar;
class QPushButton;

class LayoutItem;
class LayoutProfile;
class LayoutManager;
class CPDSettings;
class CPDController;
class SwitchBoard;

class MapWidget;
class TimeWidget;
class FuelWidget;
class GearWidget;
class FlapsWidget;
class CommsWidget;
class EngineWidget;
class TrafficWidget;


class CPDWidget : public QFrame {
  Q_OBJECT

public:
  CPDWidget(CPDSettings* _cpdSettings, int _slaveID = -1, QFrame* _parent=0);
  ~CPDWidget();

  SwitchBoard* getSwitchBoard() const;

public slots:
  void update();
  void swapPFDAltGauges(bool checked);
  void swapGearWidget(float retractable); // actuall a bool value (0|1)

private:
  CPDSettings*   cpdSettings;
  CPDController* cpdC;
  ACMap*         acMap;
  int            numEngines;
  int            slaveID; // current tab's slaveID

  // Main layout for central widget
  LayoutProfile* layoutProfile;
  LayoutManager* layoutManager;

  // Individual widgets
  // NOTE: to add a new widget, first add a forward declare above, then add
  // a line here.
  MapWidget*     mapW;
  TimeWidget*    timeW;
  FuelWidget*    fuelW;
  GearWidget*    gearW;
  FlapsWidget*   flapW;
  CommsWidget*   comW;
  EngineWidget*  engW;
  TrafficWidget* tfcW;

  // PFD Alternate - guages instead of PFD
  QWidget*       pfdAltGuages;

  void setupPFDAltGuages();
};

#endif
