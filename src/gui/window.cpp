#include "window.h"

#include <QToolBar>
#include <QPushButton>
#include <QGridLayout>
#include <QDebug>
#include <QFile>
//#include <QLatin1String>

#include "hddcontroller.h"
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


HDDWindow::HDDWindow(HDDSettings* _hddSettings, QObject* _parent) 
: QMainWindow(),
  hddSettings(_hddSettings)
{
   hddC = new HDDController(hddSettings, this);
   acMap = hddC->getACMap();
   
   QFile ss(":/style/style.css");
   ss.open(QFile::ReadOnly);
   QString style = QLatin1String(ss.readAll());
   setStyleSheet(style);
   
   layout = new QGridLayout();
   layout->setContentsMargins(0, 0, 0, 0);
   
   centralWidget = new QWidget();
   centralWidget->setLayout(layout);
   centralWidget->setContentsMargins(0, 0, 0, 0);
   layout->addWidget((QWidget*) hddC->getEngC()->getWidget(), 0, 0, 6, hddC->getNumEngines());
   layout->addWidget((QWidget*) hddC->getPFDC()->getWidget(), 0, 1+hddC->getNumEngines(), 6, 6);
   layout->addWidget((QWidget*) hddC->getMapC()->getWidget(), 0, 7+hddC->getNumEngines(), 6, 6);
   layout->addWidget((QWidget*) hddC->getComC()->getWidget(), 6, 1+hddC->getNumEngines(), 1, 6);
   layout->addWidget((QWidget*) hddC->getTfcC()->getWidget(), 6, 7+hddC->getNumEngines(), 1, 6);
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
   connect(pfdButton, SIGNAL(toggled(bool)), this, SLOT(pfdButtonClicked(bool)));
   pfdButton->setChecked(true);
   toolbar->addWidget(pfdButton);
   
   weatherButton = createToolButton("WX", false);
   weatherButton->setEnabled(false);
   toolbar->addWidget(weatherButton);
   
   trafficButton = createToolButton("TFC", true);
   trafficButton->setEnabled(true);
   connect(trafficButton, SIGNAL(toggled(bool)), hddC->getMapC(), SLOT(displayTraffic(bool)));
   trafficButton->setChecked(true);
   toolbar->addWidget(trafficButton);
   
   terrainButton = createToolButton("SAT", true);
   terrainButton->setEnabled(true);
   connect(terrainButton, SIGNAL(toggled(bool)), hddC->getMapView(), SLOT(showSatMap(bool)));
   terrainButton->setChecked(true);
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
   connect(zoomInButton, SIGNAL(clicked()), hddC->getMapC(), SLOT(increaseZoom()));
   // If you are at the max zoom (very close shot of ground), disable zoom in button
   connect(hddC->getMapC(), SIGNAL(zoomMaxReached(bool)), zoomInButton, SLOT(setDisabled(bool)));
   connect(hddC->getMapC(), SIGNAL(zoomEither(bool)),     zoomInButton, SLOT(setEnabled(bool)));
   toolbar->addWidget(zoomInButton);
   
   zoomOutButton = createToolButton("Zoom Out", false);
   zoomOutButton->setEnabled(true);
   connect(zoomOutButton, SIGNAL(clicked()), hddC->getMapC(), SLOT(decreaseZoom()));
   // If you are at the minimum zoom (whole earth), disable zoom out button
   connect(hddC->getMapC(), SIGNAL(zoomMinReached(bool)), zoomOutButton, SLOT(setDisabled(bool)));
   connect(hddC->getMapC(), SIGNAL(zoomEither(bool)),     zoomOutButton, SLOT(setEnabled(bool)));
   toolbar->addWidget(zoomOutButton);
   
   this->addToolBar(Qt::RightToolBarArea, toolbar);
}

void HDDWindow::pfdButtonClicked(bool checked)
{
   QWidget* mapW = (QWidget*) hddC->getMapC()->getWidget();
   QWidget* adiW = (QWidget*) hddC->getADIC()->getWidget();
   QWidget* altW = (QWidget*) hddC->getALTC()->getWidget();
   QWidget* asiW = (QWidget*) hddC->getASIC()->getWidget();
   QWidget* hsiW = (QWidget*) hddC->getHSIC()->getWidget();
   QWidget* pfdW = (QWidget*) hddC->getPFDC()->getWidget();
   QWidget* tcdW = (QWidget*) hddC->getTCDC()->getWidget();
   QWidget* vsiW = (QWidget*) hddC->getVSIC()->getWidget();
   
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

      layout->addWidget(pfdW, 0, 1+hddC->getNumEngines(), 6, 6);
      pfdW->show();
   }
   // Disply other instruments if unchecked
   else {
      layout->removeWidget(pfdW);
      pfdW->hide();
      
      layout->addWidget(asiW, 1, 1+hddC->getNumEngines(), 2, 2);
      layout->addWidget(adiW, 1, 3+hddC->getNumEngines(), 2, 2);
      layout->addWidget(altW, 1, 5+hddC->getNumEngines(), 2, 2);
      layout->addWidget(tcdW, 4, 1+hddC->getNumEngines(), 2, 2);
      layout->addWidget(hsiW, 4, 3+hddC->getNumEngines(), 2, 2);
      layout->addWidget(vsiW, 4, 5+hddC->getNumEngines(), 2, 2);
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
   hddC->getMapC()->setOrientation(checked ? NORTH_UP : TRACK_UP);
}

