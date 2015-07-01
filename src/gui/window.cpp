#include "window.h"

#include <QToolBar>
#include <QPushButton>
#include <QGridLayout>
#include <QDebug>

#include "core/hddsettings.h"

#include "map/mapcontroller.h"
#include "instruments/adicontroller.h"
#include "instruments/altcontroller.h"
#include "instruments/asicontroller.h"
#include "instruments/hsicontroller.h"
#include "instruments/pfdcontroller.h"
#include "instruments/tcdcontroller.h"
#include "instruments/vsicontroller.h"

#include "comms/commscontroller.h"
#include "engine/enginecontroller.h"
#include "traffic/trafficcontroller.h"


HDDWindow::HDDWindow(HDDSettings* _hddSettings, ACMap* _acMap, QObject* _parent) 
: QMainWindow(),
  hddSettings(_hddSettings),
  acMap(_acMap)
{
   mapC = new MapController(hddSettings, acMap, this);
   adiC = new ADIController(this);
   altC = new ALTController(this);
   asiC = new ASIController(this);
   hsiC = new HSIController(this);
   pfdC = new PFDController(this);
   tcdC = new TCDController(this);
   vsiC = new VSIController(this);
   
   int numEngines = 2;
   comC = new CommsController(hddSettings, this);
   engC = new EngineController(hddSettings, numEngines, this);
   tfcC = new TrafficController(hddSettings, acMap, this);
   
   layout = new QGridLayout();
   
   centralWidget = new QWidget();
   centralWidget->setLayout(layout);
   centralWidget->setContentsMargins(0, 0, 0, 0);
   layout->addWidget((QWidget*) engC->getWidget(), 0, 0, 6, numEngines);
   layout->addWidget((QWidget*) pfdC->getWidget(), 0, 1+numEngines, 6, 6);
   layout->addWidget((QWidget*) mapC->getWidget(), 0, 7+numEngines, 6, 6);
   layout->addWidget((QWidget*) comC->getWidget(), 6, 1+numEngines, 1, 6);
   layout->addWidget((QWidget*) tfcC->getWidget(), 6, 7+numEngines, 1, 6);
   setCentralWidget(centralWidget);
   
   setupToolbar();
   
   setMinimumSize(QSize(1160, 585));
//   showWindow();
}

HDDWindow::~HDDWindow() {
}

/*
 * Helper function for creating consistent toolbar buttons.
 */
QPushButton* HDDWindow::createToolButton(QString text, bool checkable)
{
   QPushButton* button = new QPushButton(text);
   button->setMinimumSize(QSize(80, 80));
   QSizePolicy sp(QSizePolicy::Fixed, QSizePolicy::Fixed);
   button->setSizePolicy(sp);
   
   if (checkable) {
      button->setCheckable(true);
   }
   
   return button;
}

void HDDWindow::setupToolbar()
{
   toolbar = new QToolBar(this);
   toolbar->setContentsMargins(0, 0, 0, 0);
   toolbar->setOrientation(Qt::Vertical);// | Qt::Horizontal);
   
   pfdButton = createToolButton("PFD", true);
   pfdButton->setEnabled(true);
   pfdButton->setChecked(true);
   connect(pfdButton, SIGNAL(toggled(bool)), this, SLOT(pfdButtonClicked(bool)));
   toolbar->addWidget(pfdButton);
   
   weatherButton = createToolButton("WX", false);
   weatherButton->setEnabled(false);
   toolbar->addWidget(weatherButton);
   
   trafficButton = createToolButton("TFC", false);
   trafficButton->setEnabled(false);
   toolbar->addWidget(trafficButton);
   
   terrainButton = createToolButton("TER", false);
   terrainButton->setEnabled(false);
   toolbar->addWidget(terrainButton);
   
   orientationButton = createToolButton("North Up", true);
   orientationButton->setEnabled(true);
   orientationButton->setChecked(true);
   if (hddSettings->mapOrientation() == TRACK_UP) {
      orientationButton->setChecked(false);
   }
   connect(orientationButton, SIGNAL(toggled(bool)), this, SLOT(orientationButtonClicked(bool)));
   toolbar->addWidget(orientationButton);
   
   zoomInButton = createToolButton("Zoom In", false);
   zoomInButton->setEnabled(true);
   connect(zoomInButton, SIGNAL(clicked()), mapC, SLOT(increaseZoom()));
   // If you are at the max zoom (very close shot of ground), disable zoom in button
   connect(mapC, SIGNAL(zoomMaxReached(bool)), zoomInButton, SLOT(setDisabled(bool)));
   connect(mapC, SIGNAL(zoomEither(bool)),     zoomInButton, SLOT(setEnabled(bool)));
   toolbar->addWidget(zoomInButton);
   
   zoomOutButton = createToolButton("Zoom Out", false);
   zoomOutButton->setEnabled(true);
   connect(zoomOutButton, SIGNAL(clicked()), mapC, SLOT(decreaseZoom()));
   // If you are at the minimum zoom (whole earth), disable zoom out button
   connect(mapC, SIGNAL(zoomMinReached(bool)), zoomOutButton, SLOT(setDisabled(bool)));
   connect(mapC, SIGNAL(zoomEither(bool)),     zoomOutButton, SLOT(setEnabled(bool)));
   toolbar->addWidget(zoomOutButton);
   
   this->addToolBar(Qt::RightToolBarArea, toolbar);
}

void HDDWindow::pfdButtonClicked(bool checked)
{
   QWidget* mapW = (QWidget*) mapC->getWidget();
   QWidget* adiW = (QWidget*) adiC->getWidget();
   QWidget* altW = (QWidget*) altC->getWidget();
   QWidget* asiW = (QWidget*) asiC->getWidget();
   QWidget* hsiW = (QWidget*) hsiC->getWidget();
   QWidget* pfdW = (QWidget*) pfdC->getWidget();
   QWidget* tcdW = (QWidget*) tcdC->getWidget();
   QWidget* vsiW = (QWidget*) vsiC->getWidget();
   
   // Display PFD if checked
   if (checked) {
      layout->removeWidget(adiW);
      layout->removeWidget(altW);
      layout->removeWidget(asiW);
      layout->removeWidget(hsiW);
      layout->removeWidget(tcdW);
      layout->removeWidget(vsiW);
      adiW->hide();
      altW->hide();
      asiW->hide();
      hsiW->hide();
      tcdW->hide();
      vsiW->hide();

      layout->addWidget(pfdW, 0, 1, 3, 3);
      pfdW->show();
   }
   // Disply other instruments if unchecked
   else {
      layout->removeWidget(pfdW);
      pfdW->hide();
      
      layout->addWidget(asiW, 0, 1);
      layout->addWidget(adiW, 0, 2);
      layout->addWidget(altW, 0, 3);
      layout->addWidget(tcdW, 1, 1);
      layout->addWidget(hsiW, 1, 2);
      layout->addWidget(vsiW, 1, 3);
      adiW->show();
      altW->show();
      asiW->show();
      hsiW->show();
      tcdW->show();
      vsiW->show();
   }
}

void HDDWindow::orientationButtonClicked(bool checked)
{
   mapC->setOrientation(checked ? NORTH_UP : TRACK_UP);
}

void HDDWindow::latLonUpdate(float lat, float lon)
{
   mapC->panToLocation(lat, lon);
}

