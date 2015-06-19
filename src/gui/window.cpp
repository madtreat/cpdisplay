#include "window.h"

#include <QToolBar>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDebug>

#include "hddsettings.h"

#include "mapcontroller.h"
#include "adicontroller.h"
#include "altcontroller.h"
#include "asicontroller.h"
#include "hsicontroller.h"
#include "pfdcontroller.h"
#include "tcdcontroller.h"
#include "vsicontroller.h"


HDDWindow::HDDWindow(HDDSettings* _hddSettings, QObject* _parent) 
: QMainWindow(),
  hddSettings(_hddSettings)
{
   mapC = new MapController(hddSettings, this);
   adiC = new ADIController(this);
   altC = new ALTController(this);
   asiC = new ASIController(this);
   hsiC = new HSIController(this);
   pfdC = new PFDController(this);
   tcdC = new TCDController(this);
   vsiC = new VSIController(this);
   
   layout = new QHBoxLayout();
   
   centralWidget = new QWidget();
   centralWidget->setLayout(layout);
   layout->addWidget((QWidget*) pfdC->getWidget());
   layout->addWidget((QWidget*) mapC->getWidget());
   setCentralWidget(centralWidget);
   
   setupToolbar();
   
   setMinimumSize(QSize(1280, 620));
//   showWindow();
}

HDDWindow::~HDDWindow() {
}

void HDDWindow::setupToolbar()
{
   toolbar = new QToolBar(this);
   toolbar->setOrientation(Qt::Vertical);
   
   pfdButton = new QPushButton("PFD");
   pfdButton->setEnabled(false);
   pfdButton->setMinimumHeight(80);
   toolbar->addWidget(pfdButton);
   
   weatherButton = new QPushButton("WX");
   weatherButton->setEnabled(false);
   weatherButton->setMinimumHeight(80);
   toolbar->addWidget(weatherButton);
   
   trafficButton = new QPushButton("TFC");
   trafficButton->setEnabled(false);
   trafficButton->setMinimumHeight(80);
   toolbar->addWidget(trafficButton);
   
   terrainButton = new QPushButton("TER");
   terrainButton->setEnabled(false);
   terrainButton->setMinimumHeight(80);
   toolbar->addWidget(terrainButton);
   
   orientationButton = new QPushButton("North Up");
   orientationButton->setEnabled(true);
   orientationButton->setMinimumHeight(80);
   orientationButton->setCheckable(true);
   orientationButton->setChecked(true);
   if (hddSettings->mapOrientation() == TRACK_UP) {
      orientationButton->setChecked(false);
   }
   connect(orientationButton, SIGNAL(toggled(bool)), this, SLOT(orientationButtonClicked(bool)));
   toolbar->addWidget(orientationButton);
   
   zoomInButton = new QPushButton("Zoom In");
   zoomInButton->setEnabled(true);
   zoomInButton->setMinimumHeight(80);
   connect(zoomInButton, SIGNAL(clicked()), mapC, SLOT(increaseZoom()));
   // If you are at the max zoom (very close shot of ground), disable zoom in button
   connect(mapC, SIGNAL(zoomMaxReached(bool)), zoomInButton, SLOT(setDisabled(bool)));
   connect(mapC, SIGNAL(zoomEither(bool)),     zoomInButton, SLOT(setEnabled(bool)));
   toolbar->addWidget(zoomInButton);
   
   zoomOutButton = new QPushButton("Zoom Out");
   zoomOutButton->setEnabled(true);
   zoomOutButton->setMinimumHeight(80);
   connect(zoomOutButton, SIGNAL(clicked()), mapC, SLOT(decreaseZoom()));
   // If you are at the minimum zoom (whole earth), disable zoom out button
   connect(mapC, SIGNAL(zoomMinReached(bool)), zoomOutButton, SLOT(setDisabled(bool)));
   connect(mapC, SIGNAL(zoomEither(bool)),     zoomOutButton, SLOT(setEnabled(bool)));
   toolbar->addWidget(zoomOutButton);
   
   this->addToolBar(Qt::RightToolBarArea, toolbar);
}

void HDDWindow::orientationButtonClicked(bool checked)
{
   mapC->setOrientation(checked ? NORTH_UP : TRACK_UP);
}

void HDDWindow::latLonUpdate(float lat, float lon, int ac)
{
   mapC->panToLocation(lat, lon, ac);
}

