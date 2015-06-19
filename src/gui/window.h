#include <QMainWindow>

class QToolBar;
class QPushButton;
class QHBoxLayout;

class MapController;
class PFDController;
class ALTController;

class HDDWindow : public QMainWindow {
   Q_OBJECT

public:
   HDDWindow(QObject* _parent=0);
   ~HDDWindow();
   
   void setupToolbar();

public slots:
   void latLonUpdate(float lat, float lon, int ac=0);
   void altMSLUpdate(float alt);
   void altAGLUpdate(float alt);

private:
   MapController* mapC;
   PFDController* pfdC;
   ALTController* altC;
   
   // Main layout for central widget
   QHBoxLayout*   layout;
   QWidget*       centralWidget;
   
   // The toolbar and buttons
   QToolBar*      toolbar;
   QPushButton*   pfdButton;
   QPushButton*   weatherButton;
   QPushButton*   trafficButton;
   QPushButton*   terrainButton;
   QPushButton*   orientationButton; // map orientation
   QPushButton*   zoomInButton;
   QPushButton*   zoomOutButton;
//   QPushButton*    homeButton;
};
