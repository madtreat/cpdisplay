#include "window.h"

#include <QToolBar>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QFile>
#include <QWidget>
//#include <QLatin1String>

#include "qt-layout/layoutitem.h"
#include "qt-layout/layoutprofile.h"
#include "qt-layout/layoutmanager.h"

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
#include "comms/commswidget.h"
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
   
   layoutProfile = new LayoutProfile(hddSettings->layoutProfile());
   layoutManager = new LayoutManager();
   
   comW = new CommsWidget(hddSettings, hddC->getComC());
   
   setupPFDAltGuages();
   setupToolbar();
//   pfdButtonClicked(true); // Hide the alt guages after creation;
   
   QMap<int, QWidget*> widgetMap;
   widgetMap.insert(1, hddC->getPFDC()->getWidget());
   widgetMap.insert(2, hddC->getMapC()->getWidget());
   widgetMap.insert(3, hddC->getEngC()->getWidget());
   widgetMap.insert(4, hddC->getTfcC()->getWidget());
   widgetMap.insert(5, comW);
   widgetMap.insert(6, toolbar);
   widgetMap.insert(7, pfdAltGuages);
   
   for (int i = 0; i < layoutProfile->numItems(); i++) {
      LayoutItem* item = layoutProfile->itemAt(i);
      if (!item) {
         qWarning() << "Warning: null item";
         continue;
      }
      item->widget = widgetMap.value(i+1);
      layoutMap.insert(i+1, item);
      layoutManager->addWidget(item);
   }
   
   setCentralWidget(layoutManager);
   
   setMinimumSize(QSize(1160, 590));
}

HDDWindow::~HDDWindow()
{
   delete hddC;
   delete acMap;
   delete layoutManager;
   delete hddSettings;
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
   toolbar = new QFrame(this);
   QVBoxLayout* tblayout = new QVBoxLayout(toolbar);
   tblayout->setContentsMargins(0, 0, 0, 0);
//   toolbar->setOrientation(Qt::Vertical);// | Qt::Horizontal);

   pfdButton = createToolButton("PFD", true);
   pfdButton->setEnabled(true);
   connect(pfdButton, SIGNAL(toggled(bool)), this, SLOT(pfdButtonClicked(bool)));
   pfdButton->setChecked(true);
   tblayout->addWidget(pfdButton);
   
   weatherButton = createToolButton("WX", false);
   weatherButton->setEnabled(false);
   tblayout->addWidget(weatherButton);
   
   trafficButton = createToolButton("TFC", true);
   trafficButton->setEnabled(true);
   connect(trafficButton, SIGNAL(toggled(bool)), hddC->getMapC(), SLOT(displayTraffic(bool)));
   trafficButton->setChecked(true);
   tblayout->addWidget(trafficButton);
   
   terrainButton = createToolButton("SAT", true);
   terrainButton->setEnabled(true);
   connect(terrainButton, SIGNAL(toggled(bool)), hddC->getMapView(), SLOT(showSatMap(bool)));
   connect(terrainButton, SIGNAL(toggled(bool)), hddC->getOverlay(), SLOT(satButtonClicked(bool)));
   terrainButton->setChecked(true);
   tblayout->addWidget(terrainButton);
   
   orientationButton = createToolButton("North Up", true);
   orientationButton->setEnabled(true);
   orientationButton->setChecked(true);
   if (hddSettings->mapOrientation() == TRACK_UP) {
      orientationButton->setChecked(false);
   }
   connect(orientationButton, SIGNAL(toggled(bool)), this, SLOT(orientationButtonClicked(bool)));
   tblayout->addWidget(orientationButton);
   
   zoomInButton = createToolButton("Zoom In", false);
   zoomInButton->setEnabled(true);
   connect(zoomInButton, SIGNAL(clicked()), hddC->getMapC(), SLOT(increaseZoom()));
   // If you are at the max zoom (very close shot of ground), disable zoom in button
   connect(hddC->getMapC(), SIGNAL(zoomMaxReached(bool)), zoomInButton, SLOT(setDisabled(bool)));
   connect(hddC->getMapC(), SIGNAL(zoomEither(bool)),     zoomInButton, SLOT(setEnabled(bool)));
   tblayout->addWidget(zoomInButton);
   
   zoomOutButton = createToolButton("Zoom Out", false);
   zoomOutButton->setEnabled(true);
   connect(zoomOutButton, SIGNAL(clicked()), hddC->getMapC(), SLOT(decreaseZoom()));
   // If you are at the minimum zoom (whole earth), disable zoom out button
   connect(hddC->getMapC(), SIGNAL(zoomMinReached(bool)), zoomOutButton, SLOT(setDisabled(bool)));
   connect(hddC->getMapC(), SIGNAL(zoomEither(bool)),     zoomOutButton, SLOT(setEnabled(bool)));
   tblayout->addWidget(zoomOutButton);
   
   // No longer using the toolbar directly in the QMainWindow so it can be
   // inserted into the LayoutManager.
//   this->addToolBar(Qt::RightToolBarArea, toolbar);
}

void HDDWindow::setupPFDAltGuages()
{
   QWidget* adiW = (QWidget*) hddC->getADIC()->getWidget();
   QWidget* altW = (QWidget*) hddC->getALTC()->getWidget();
   QWidget* asiW = (QWidget*) hddC->getASIC()->getWidget();
   QWidget* hsiW = (QWidget*) hddC->getHSIC()->getWidget();
   QWidget* tcdW = (QWidget*) hddC->getTCDC()->getWidget();
   QWidget* vsiW = (QWidget*) hddC->getVSIC()->getWidget();
   
   pfdAltGuages = new QWidget(this);
   QVBoxLayout* vbLayout = new QVBoxLayout(pfdAltGuages);
   QHBoxLayout* topRow = new QHBoxLayout();
   QHBoxLayout* botRow = new QHBoxLayout();
   
   topRow->addWidget(asiW);
   topRow->addWidget(adiW);
   topRow->addWidget(altW);
   botRow->addWidget(tcdW);
   botRow->addWidget(hsiW);
   botRow->addWidget(vsiW);
   
   vbLayout->addLayout(topRow);
   vbLayout->addLayout(botRow);
}

void HDDWindow::pfdButtonClicked(bool checked)
{
   LayoutItem* pfd = layoutProfile->getItemByName("PFDWidget");
   LayoutItem* pfdAlt = layoutProfile->getItemByName("PFDAltGuages");
   if (pfd == NULL || pfd->widget == NULL) {
//      qWarning() << "Warning: PFDWidget LayoutItem was not found in the profile.";
      return;
   }
   if (pfdAlt == NULL || pfdAlt->widget == NULL) {
//      qWarning() << "Warning: PFDAltWidget LayoutItem was not found in the profile.";
      return;
   }
   
   // Display PFD if checked
   if (checked) {
      layoutManager->replaceItem(pfdAlt, pfd);
   }
   // Display other instruments if unchecked
   else {
      layoutManager->replaceItem(pfd, pfdAlt);
   }
}

void HDDWindow::orientationButtonClicked(bool checked)
{
   hddC->getMapC()->setOrientation(checked ? NORTH_UP : TRACK_UP);
}

