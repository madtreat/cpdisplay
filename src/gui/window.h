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
class CommsWidget;
class EngineWidget;


class CPDWindow : public QMainWindow {
   Q_OBJECT

public:
   CPDWindow(CPDSettings* _cpdSettings, QObject* _parent=0);
   ~CPDWindow();

public slots:
   void pfdButtonClicked(bool checked);
   
private:
   CPDSettings*   cpdSettings;
   CPDController* cpdC;
   ACMap*         acMap;
   int            numEngines;
   
   // Main layout for central widget
   LayoutProfile* layoutProfile;
   LayoutManager* layoutManager;
   QMap<int, LayoutItem*> layoutMap;
   
   // Individual widgets
   MapWidget*     mapW;
   TimeWidget*    timeW;
   FuelWidget*    fuelW;
   GearWidget*    gearW;
   CommsWidget*   comW;
   EngineWidget*  engW;
   
   // PFD Alternate - guages instead of PFD
   QWidget*       pfdAltGuages;
   
   // The toolbar and buttons
//   QToolBar*      toolbar;
   QFrame*        toolbar;
   QPushButton*   pfdButton;

   void setupToolbar();
   void setupPFDAltGuages();
};
