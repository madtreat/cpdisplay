#include <QMainWindow>

#include "core/aircraft.h"
#include "map/mapcontroller.h"

class QToolBar;
class QPushButton;

class LayoutItem;
class LayoutProfile;
class LayoutManager;
class HDDSettings;
class HDDController;

class CommsWidget;


class HDDWindow : public QMainWindow {
   Q_OBJECT

public:
   HDDWindow(HDDSettings* _hddSettings, QObject* _parent=0);
   ~HDDWindow();

public slots:
   void pfdButtonClicked(bool checked);
   void orientationButtonClicked(bool checked);
   
private:
   HDDSettings*   hddSettings;
   HDDController* hddC;
   ACMap*         acMap;
   int            numEngines;
   
   // Main layout for central widget
   LayoutProfile* layoutProfile;
   LayoutManager* layoutManager;
   QMap<int, LayoutItem*> layoutMap;
   
   // Individual widgets
   CommsWidget*   comW;
   
   // PFD Alternate - guages instead of PFD
   QWidget*       pfdAltGuages;
   
   // The toolbar and buttons
   QToolBar*      toolbar;
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
