#include <QMainWindow>

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

class MapWidget;
class TimeWidget;
class FuelWidget;
class GearWidget;
class FlapsWidget;
class CommsWidget;
class EngineWidget;
class TrafficWidget;


class CPDWindow : public QMainWindow {
   Q_OBJECT

public:
   CPDWindow(CPDSettings* _cpdSettings, QObject* _parent=0);
   ~CPDWindow();

public slots:
   void swapPFDAltGauges(bool checked);
   void swapGearWidget(float retractable); // actuall a bool value (0|1)
   
private:
   CPDSettings*   cpdSettings;
   CPDController* cpdC;
   ACMap*         acMap;
   int            numEngines;
   
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
