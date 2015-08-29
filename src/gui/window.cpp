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

   // Blanks and alternate-blank widgets
   QFrame* gearWBlank = new QFrame();
   gearWBlank->setObjectName("border");
   connect(cpdC, &CPDC::updateGearRetractable, this, &CPDWindow::swapGearWidget);

   QFrame* pfdFillerBlank = new QFrame();
   pfdFillerBlank->setObjectName("border");
   QFrame* blank1 = new QFrame();
   QFrame* blank2 = new QFrame();
   QFrame* blank3 = new QFrame();
   blank1->setObjectName("border");
   blank2->setObjectName("border");
   blank3->setObjectName("border");

   setupPFDAltGuages();

   // NOTE: to add a new widget, insert it into the map below
   QMap<QString, QWidget*> widgetMap;
   widgetMap.insert("PFDWidget",       cpdC->getPFDC()->getWidget());
   widgetMap.insert("MapWidget",       mapW);
   widgetMap.insert("EngineWidget",    engW);
   widgetMap.insert("TrafficWidget",   tfcW);
   widgetMap.insert("CommsWidget",     comW);
   widgetMap.insert("PFDAltGuages",    pfdAltGuages);
   widgetMap.insert("FuelWidget",      fuelW);
   widgetMap.insert("TimeWidget",      timeW);
   widgetMap.insert("GearWidget",      gearW);
   widgetMap.insert("GearWidgetBlank", gearWBlank);
   widgetMap.insert("FlapsWidget",     flapW);
   widgetMap.insert("PFDFillerBlank",  pfdFillerBlank);
   widgetMap.insert("Blank1",          blank1);
   widgetMap.insert("Blank2",          blank2);
   widgetMap.insert("Blank3",          blank3);
   
   foreach (QString widgetName, widgetMap.keys()) {
      LayoutItem* item = layoutProfile->getItemByName(widgetName);
      if (!item) {
         qWarning() << "Warning: tried to add a null LayoutItem to LayoutManager";
         qWarning() << "   Name:" << widgetName;
         continue;
      }
      item->widget = widgetMap.value(widgetName);
      if (!item->widget) {
         qWarning() << "Warning: tried to add a null Widget to LayoutManager";
         qWarning() << "   Name:" << widgetName;
         continue;
      }
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

void CPDWindow::swapGearWidget(float retractable)
{
   LayoutItem* gw = layoutProfile->getItemByName("GearWidget");
   LayoutItem* gwb = layoutProfile->getItemByName("GearWidgetBlank");
   if (retractable) {
      layoutManager->replaceItem(gwb, gw);
   }
   else {
      layoutManager->replaceItem(gw, gwb);
   }
}

