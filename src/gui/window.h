#include <QMainWindow>

#include "map/mapcontroller.h"

class QToolBar;
class QPushButton;
class QGridLayout;

class HDDSettings;
class MapView;
class mapOverlay;

//class MapController;
class ADIController;
class ALTController;
class ASIController;
class HSIController;
class PFDController;
class TCDController;
class VSIController;

class CommsController;
class EngineController;
class TrafficController;

class HDDWindow : public QMainWindow {
   Q_OBJECT

public:
   HDDWindow(HDDSettings* _hddSettings, QObject* _parent=0);
   ~HDDWindow();

   MapController* getMapC() const { return mapC; }
   ADIController* getADIC() const { return adiC; }
   ALTController* getALTC() const { return altC; }
   ASIController* getASIC() const { return asiC; }
   HSIController* getHSIC() const { return hsiC; }
   PFDController* getPFDC() const { return pfdC; }
   TCDController* getTCDC() const { return tcdC; }
   VSIController* getVSIC() const { return vsiC; }
   
   CommsController*   getComC() const { return comC; }
   EngineController*  getEngC() const { return engC; }
   TrafficController* getTfcC() const { return tfcC; }
   
   MapView*    getMapView() const { return mapC->getMapView(); }
   MapOverlay* getOverlay() const { return mapC->getOverlay(); }

public slots:
   void pfdButtonClicked(bool checked);
   void orientationButtonClicked(bool checked);
   
   void latLonUpdate(float lat, float lon, int ac=0);

private:
   HDDSettings* hddSettings;
   
   // Widget controllers
   MapController* mapC;
   ADIController* adiC;
   ALTController* altC;
   ASIController* asiC;
   HSIController* hsiC;
   PFDController* pfdC;
   TCDController* tcdC;
   VSIController* vsiC;
   
   CommsController*   comC;
   EngineController*  engC;
   TrafficController* tfcC;
   
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

   void setupToolbar();
};
