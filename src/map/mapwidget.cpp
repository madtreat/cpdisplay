/* 
 * File:   mapwidget.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 15, 2015, 2:36 PM
 */

#include "mapwidget.h"

#include <QGridLayout>
#include <QResizeEvent>

#include "qfi/ui/LayoutSquare.h"
#include "qt-google-maps/mapsettings.h"
#include "core/cpdsettings.h"
#include "core/mapconsts.h"
#include "mapview.h"
#include "mapoverlay.h"

MapWidget::MapWidget(CPDSettings* _cpdSettings, MapSettings* _mapSettings, ACMap* _acMap, QFrame* _parent)
: QFrame(_parent),
  cpdSettings(_cpdSettings),
  mapSettings(_mapSettings),
  acMap(_acMap)
{
//   LayoutSquare* layout = new LayoutSquare(this);
   QGridLayout* layout = new QGridLayout(this);
   layout->setContentsMargins(0, 0, 0, 0);
   
   QSizePolicy sp(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
   sp.setHeightForWidth(true);
   setSizePolicy(sp);
   
   view = new MapView(cpdSettings, mapSettings, acMap);
   layout->addWidget(view);
   view->setSizePolicy(sp);
   
   overlay = new MapOverlay(cpdSettings, mapSettings, acMap, view);
   overlay->setGeometry(view->geometry());

   setMinimumSize(QSize(DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT));
   resize(DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT);
//   setObjectName("border");
}

//MapWidget::MapWidget(const MapWidget& orig)
//{
//}

MapWidget::~MapWidget()
{
   delete view;
   delete overlay;
}

QSize MapWidget::sizeHint() const
{
   return QSize(DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT);
}

void MapWidget::resizeEvent(QResizeEvent* event)
{
   resize(event->size());
}

void MapWidget::resize(int w, int h)
{
   resize(QSize(w, h));
}

void MapWidget::resize(const QSize& size)
{
   QWidget::resize(size);
   view->resize(size);
   overlay->resize(size);
}


void MapWidget::setZoom(int level)
{
   view->setZoom(level);
   overlay->setZoom(level);
}

void MapWidget::panToLocation(float lat, float lon)
{
   view->panToLocation(lat, lon);
   overlay->panToLocation(lat, lon);
   overlay->repaint();
}

void MapWidget::setOrientation(MapOrientation mo)
{
   // The MapController::setOrientation() function directly updates the settings,
   // which the overlay uses.  But the MapView class still needs to change the
   // way it follows the aircraft.
   view->setOrientation(mo);
}

