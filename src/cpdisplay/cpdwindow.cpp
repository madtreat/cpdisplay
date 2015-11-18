#include "cpdwindow.h"

#include <QMenuBar>

#include "cpdwidget.h"
#include "cpdcontroller.h"
#include "mcswidget.h"
#include "core/cpdsettings.h"
#include "core/switchboard.h"


CPDWindow::CPDWindow(CPDSettings* _cpdSettings, QWidget* _parent) 
: QMainWindow(_parent),
  settings(_cpdSettings)
{
   if (settings->isMCS()) {
      qDebug() << "Creating MCS for" << settings->numSlaves() << "slaves...";
      // slaveID = 0; // default to first in the list
      tabWidget = new QTabWidget(this);

      for (int i = 0; i < settings->numSlaves(); ++i)
      {
         CPDWidget* slave = new CPDWidget(settings, i);
         slaveSwitches.append(slave->getSwitchBoard());
         slaveCPDs.append(slave);
         tabWidget->addTab(slave, settings->getSlave(i)->m_slaveName);
      }

      mcsWidget = new MCSWidget(settings, slaveSwitches);
      tabWidget->insertTab(0, mcsWidget, "MCS");
      tabWidget->setCurrentIndex(0);

      setCentralWidget(tabWidget);
   }
   else {
      // slaveID = -1; // this is a non-MCS CPD
      mcsWidget = NULL;
      cpdWidget = new CPDWidget(settings, -1);
      setCentralWidget(cpdWidget);
   }
}

CPDWindow::~CPDWindow()
{
   delete settings;
}


void CPDWindow::displayMCSWidget()
{
   if (settings->isMCS()) {
      setCentralWidget(mcsWidget);
   }
   qDebug() << "displayMCSWidget";
}

void CPDWindow::displaySlaveCPD(int i)
{
   if (settings->isMCS()) {
      setCentralWidget(slaveCPDs.at(i));
   }
   qDebug() << "displaySlaveCPD(" << i << ")";
}

