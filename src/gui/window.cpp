#include "window.h"

#include <QToolBar>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDebug>

#include "pfdcontroller.h"
#include "mapcontroller.h"


HDDWindow::HDDWindow(QObject* _parent) 
: QMainWindow()
{
   map = new MapController(this);
   pfd = new PFDController(this);
   
   layout = new QHBoxLayout();
   
   centralWidget = new QWidget();
   centralWidget->setLayout(layout);
   layout->addWidget((QWidget*) pfd->getWidget());
   layout->addWidget((QWidget*) map->getWidget());
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
   connect(zoomInButton, SIGNAL(clicked()), map, SLOT(increaseZoom()));
   // If you are at the max zoom (very close shot of ground), disable zoom in button
   connect(map, SIGNAL(zoomMaxReached(bool)), zoomInButton, SLOT(setDisabled(bool)));
   connect(map, SIGNAL(zoomEither(bool)),     zoomInButton, SLOT(setEnabled(bool)));
   toolbar->addWidget(zoomInButton);
   
   zoomOutButton = new QPushButton("Zoom Out");
   zoomOutButton->setEnabled(true);
   zoomOutButton->setMinimumHeight(80);
   connect(zoomOutButton, SIGNAL(clicked()), map, SLOT(decreaseZoom()));
   // If you are at the minimum zoom (whole earth), disable zoom out button
   connect(map, SIGNAL(zoomMinReached(bool)), zoomOutButton, SLOT(setDisabled(bool)));
   connect(map, SIGNAL(zoomEither(bool)),     zoomOutButton, SLOT(setEnabled(bool)));
   toolbar->addWidget(zoomOutButton);
   
   this->addToolBar(Qt::RightToolBarArea, toolbar);
}

