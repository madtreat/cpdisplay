#include "window.h"

#include <QHBoxLayout>
#include <QDebug>

#include "pfdcontroller.h"
#include "mapcontroller.h"


HDDWindow::HDDWindow(QObject* _parent) 
: QMainWindow()
{
   map = new MapController(this);
   pfd = new PFDController(this);
   
   QHBoxLayout* layout = new QHBoxLayout();
   
   QWidget* centralWidget = new QWidget();
   centralWidget->setLayout(layout);
   layout->addWidget((QWidget*) pfd->getWidget());
   layout->addWidget((QWidget*) map->getWidget());
   setCentralWidget(centralWidget);
   
   setMinimumSize(QSize(1280, 620));
//   showWindow();
}

HDDWindow::~HDDWindow() {
}
