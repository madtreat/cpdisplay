#include <QMainWindow>

class QToolBar;
class QPushButton;
class QHBoxLayout;

class HDDSettings;
class MapController;
class PFDController;
class ALTController;
class HSIController;

class HDDWindow : public QMainWindow {
   Q_OBJECT

public:
   HDDWindow(HDDSettings* _hddSettings, QObject* _parent=0);
   ~HDDWindow();
   
   void setupToolbar();

public slots:
   void orientationButtonClicked(bool checked);
   
   void compassUpdate(float heading);
   void latLonUpdate(float lat, float lon, int ac=0);
   void altMSLUpdate(float alt);
   void altAGLUpdate(float alt);

private:
   HDDSettings* hddSettings;
   
   MapController* mapC;
   PFDController* pfdC;
   ALTController* altC;
   HSIController* hsiC;
   
   // Main layout for central widget
   QHBoxLayout*   layout;
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
};
