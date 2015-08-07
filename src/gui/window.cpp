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

#include "cpdcontroller.h"
#include "core/cpdsettings.h"

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
#include "engine/enginewidget.h"
#include "traffic/trafficcontroller.h"


CPDWindow::CPDWindow(CPDSettings* _cpdSettings, QObject* _parent) 
: QMainWindow(),
  cpdSettings(_cpdSettings)
{
   cpdC = new CPDController(cpdSettings, this);
   acMap = cpdC->getACMap();
   
   QFile ss(":/style/style.css");
   ss.open(QFile::ReadOnly);
   QString style = QLatin1String(ss.readAll());
   setStyleSheet(style);
   
   layoutProfile = new LayoutProfile(cpdSettings->layoutProfile());
   layoutManager = new LayoutManager();
   
   engW = new EngineWidget(cpdSettings, cpdC->getEngC());
   comW = new CommsWidget(cpdSettings, cpdC->getComC());
   
   setupPFDAltGuages();
   setupToolbar();
//   pfdButtonClicked(true); // Hide the alt guages after creation;
   
   QMap<int, QWidget*> widgetMap;
   widgetMap.insert(1, cpdC->getPFDC()->getWidget());
   widgetMap.insert(2, cpdC->getMapC()->getWidget());
   widgetMap.insert(3, engW);
   widgetMap.insert(4, cpdC->getTfcC()->getWidget());
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
   
   setMinimumSize(QSize(1344, 756));
}

CPDWindow::~CPDWindow()
{
   delete cpdC;
   delete acMap;
   delete layoutManager;
   delete cpdSettings;
}

/*
 * Helper function for creating consistent toolbar buttons.
 */
QPushButton* CPDWindow::createToolButton(QString text, bool checkable)
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

void CPDWindow::setupToolbar()
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
   connect(trafficButton, SIGNAL(toggled(bool)), cpdC->getMapC(), SLOT(displayTraffic(bool)));
   trafficButton->setChecked(true);
   tblayout->addWidget(trafficButton);
   
   terrainButton = createToolButton("SAT", true);
   terrainButton->setEnabled(true);
   connect(terrainButton, SIGNAL(toggled(bool)), cpdC->getMapView(), SLOT(showSatMap(bool)));
   connect(terrainButton, SIGNAL(toggled(bool)), cpdC->getOverlay(), SLOT(satButtonClicked(bool)));
   terrainButton->setChecked(true);
   tblayout->addWidget(terrainButton);
   
   orientationButton = createToolButton("North Up", true);
   orientationButton->setEnabled(true);
   orientationButton->setChecked(true);
   if (cpdSettings->mapOrientation() == TRACK_UP) {
      orientationButton->setChecked(false);
   }
   connect(orientationButton, SIGNAL(toggled(bool)), this, SLOT(orientationButtonClicked(bool)));
   tblayout->addWidget(orientationButton);
   
   zoomInButton = createToolButton("Zoom In", false);
   zoomInButton->setEnabled(true);
   connect(zoomInButton, SIGNAL(clicked()), cpdC->getMapC(), SLOT(increaseZoom()));
   // If you are at the max zoom (very close shot of ground), disable zoom in button
   connect(cpdC->getMapC(), SIGNAL(zoomMaxReached(bool)), zoomInButton, SLOT(setDisabled(bool)));
   connect(cpdC->getMapC(), SIGNAL(zoomEither(bool)),     zoomInButton, SLOT(setEnabled(bool)));
   tblayout->addWidget(zoomInButton);
   
   zoomOutButton = createToolButton("Zoom Out", false);
   zoomOutButton->setEnabled(true);
   connect(zoomOutButton, SIGNAL(clicked()), cpdC->getMapC(), SLOT(decreaseZoom()));
   // If you are at the minimum zoom (whole earth), disable zoom out button
   connect(cpdC->getMapC(), SIGNAL(zoomMinReached(bool)), zoomOutButton, SLOT(setDisabled(bool)));
   connect(cpdC->getMapC(), SIGNAL(zoomEither(bool)),     zoomOutButton, SLOT(setEnabled(bool)));
   tblayout->addWidget(zoomOutButton);
   
   // No longer using the toolbar directly in the QMainWindow so it can be
   // inserted into the LayoutManager.
//   this->addToolBar(Qt::RightToolBarArea, toolbar);
}

void CPDWindow::setupPFDAltGuages()
{
   QWidget* adiW = (QWidget*) cpdC->getADIC()->getWidget();
   QWidget* altW = (QWidget*) cpdC->getALTC()->getWidget();
   QWidget* asiW = (QWidget*) cpdC->getASIC()->getWidget();
   QWidget* hsiW = (QWidget*) cpdC->getHSIC()->getWidget();
   QWidget* tcdW = (QWidget*) cpdC->getTCDC()->getWidget();
   QWidget* vsiW = (QWidget*) cpdC->getVSIC()->getWidget();
   
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

void CPDWindow::pfdButtonClicked(bool checked)
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

void CPDWindow::orientationButtonClicked(bool checked)
{
   cpdC->getMapC()->setOrientation(checked ? NORTH_UP : TRACK_UP);
}

