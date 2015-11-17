#include "cpdwindow.h"

#include <QMenuBar>

#include "cpdwidget.h"
#include "core/cpdsettings.h"

CPDWindow::CPDWindow(CPDSettings* _cpdSettings, QWidget* _parent) 
: QMainWindow(_parent),
  settings(_cpdSettings)
{
   if (settings->isMCS()) {
   	qDebug() << "Creating MCS for" << settings->numSlaves() << "slaves...";
      // slaveID = 0; // default to first in the list
      tabWidget = new QTabWidget(this);

      // mcsWidget = new MCSWidget(settings, this);
      // tabWidget->addTab(mcsWidget, "MCS");

      for (int i = 0; i < settings->numSlaves(); ++i)
      {
      	CPDWidget* slave = new CPDWidget(settings, i);
      	slaveCPDs.append(slave);
      	tabWidget->addTab(slave, settings->getSlave(i)->m_slaveName);
      }

      setCentralWidget(tabWidget);
   }
   else {
      // slaveID = -1; // this is a non-MCS CPD
      mcsWidget = NULL;
      cpdWidget = new CPDWidget(settings, -1);
      setCentralWidget(cpdWidget);
   }

   // QMenuBar* menuBar = new QMenuBar();
   // menuBar->addAction("&MCS", this, SLOT(displayMCSWidget()));

   // setMenuBar(menuBar);
}

CPDWindow::~CPDWindow() {}


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

