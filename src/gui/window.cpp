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

#include "instruments/adicontroller.h"
#include "instruments/altcontroller.h"
#include "instruments/asicontroller.h"
#include "instruments/hsicontroller.h"
#include "instruments/pfdcontroller.h"
#include "instruments/tcdcontroller.h"
#include "instruments/vsicontroller.h"

#include "map/mapwidget.h"
#include "gear/gearwidget.h"
#include "gear/flapswidget.h"
#include "comms/timewidget.h"
#include "comms/commswidget.h"
#include "engine/fuelwidget.h"
#include "engine/enginewidget.h"
#include "traffic/trafficwidget.h"


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

   MapController* mapC = cpdC->getMapC();
   // NOTE: to add a new widget, add its constructor here
   mapW  = new MapWidget(cpdSettings,     mapC->getMapSettings(), mapC, acMap);
   timeW = new TimeWidget(cpdSettings,    cpdC->getComC());
   fuelW = new FuelWidget(cpdSettings,    cpdC->getEngC());
   gearW = new GearWidget(cpdSettings,    cpdC->getGearC());
   flapW = new FlapsWidget(cpdSettings,   cpdC->getGearC());
   comW  = new CommsWidget(cpdSettings,   cpdC->getComC());
   engW  = new EngineWidget(cpdSettings,  cpdC->getEngC());
   tfcW  = new TrafficWidget(cpdSettings, cpdC->getTfcC(), acMap);
   
   setupPFDAltGuages();

   // NOTE: to add a new widget, insert it into the map below
   QMap<int, QWidget*> widgetMap;
   widgetMap.insert(1,  cpdC->getPFDC()->getWidget());
   widgetMap.insert(2,  mapW);
   widgetMap.insert(3,  engW);
   widgetMap.insert(4,  tfcW);
   widgetMap.insert(5,  comW);
   widgetMap.insert(6,  pfdAltGuages);
   widgetMap.insert(7,  fuelW);
   widgetMap.insert(8,  timeW);
   widgetMap.insert(9,  gearW);
   widgetMap.insert(10, flapW);
   
   for (int i = 0; i < layoutProfile->numItems(); i++) {
      LayoutItem* item = layoutProfile->itemAt(i);
      if (!item) {
         qWarning() << "Warning: tried to add a null LayoutItem to LayoutManager";
         continue;
      }
      item->widget = widgetMap.value(i+1);
      if (!item->widget) {
         qWarning() << "Warning: tried to add a null Widget to LayoutManager";
         continue;
      }
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

void CPDWindow::swapPFDAltGauges(bool checked)
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

