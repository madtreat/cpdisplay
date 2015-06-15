#include <QMainWindow>

class QToolBar;
class QPushButton;
class QHBoxLayout;

class MapController;
class PFDController;

class HDDWindow : public QMainWindow {
   Q_OBJECT

public:
   HDDWindow(QObject* _parent=0);
   ~HDDWindow();
   
   void setupToolbar();

public slots:

private:
   MapController* map;
   PFDController* pfd;
   
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
