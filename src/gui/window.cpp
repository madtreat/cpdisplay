#include "window.h"

#include <QToolBar>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDebug>

#include "mapcontroller.h"
#include "pfdcontroller.h"
#include "altcontroller.h"


HDDWindow::HDDWindow(QObject* _parent) 
: QMainWindow()
{
   mapC = new MapController(this);
   pfdC = new PFDController(this);
   altC = new ALTController(this);
   
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
   
   orientationButton = new QPushButton("Track Up");
   orientationButton->setEnabled(true);
   orientationButton->setMinimumHeight(80);
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

void HDDWindow::latLonUpdate(float lat, float lon, int ac)
{
   mapC->panToLocation(lat, lon, ac);
}

void HDDWindow::altMSLUpdate(float alt)
{
   pfdC->setAltitude(alt);
   altC->setAltitude(alt);
}

void HDDWindow::altAGLUpdate(float alt)
{
}
