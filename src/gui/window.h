#include <QMainWindow>

#include "core/aircraft.h"
#include "map/mapcontroller.h"

class QToolBar;
class QPushButton;

class LayoutItem;
class LayoutProfile;
class LayoutManager;
class CPDSettings;
class CPDController;

class FuelWidget;
class CommsWidget;
class EngineWidget;


class CPDWindow : public QMainWindow {
   Q_OBJECT

public:
   CPDWindow(CPDSettings* _cpdSettings, QObject* _parent=0);
   ~CPDWindow();

public slots:
   void pfdButtonClicked(bool checked);
   void orientationButtonClicked(bool checked);
   
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
   FuelWidget*    fulW;
   CommsWidget*   comW;
   EngineWidget*  engW;
   
   // PFD Alternate - guages instead of PFD
   QWidget*       pfdAltGuages;
   
   // The toolbar and buttons
//   QToolBar*      toolbar;
   QFrame*        toolbar;
   QPushButton*   pfdButton;
   QPushButton*   weatherButton;
   QPushButton*   trafficButton;
   QPushButton*   terrainButton;
   QPushButton*   orientationButton; // map orientation, checked = NORTH_UP
   QPushButton*   zoomInButton;
   QPushButton*   zoomOutButton;
//   QPushButton*    homeButton;

   QPushButton* createToolButton(QString text, bool checkable);
   void setupToolbar();
   void setupPFDAltGuages();
};
