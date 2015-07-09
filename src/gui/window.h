#include <QMainWindow>

#include "core/aircraft.h"
#include "map/mapcontroller.h"

class QToolBar;
class QPushButton;
class QGridLayout;

class HDDSettings;
class HDDController;

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
   QGridLayout*   layout;
   QWidget*       centralWidget;
   
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
};
