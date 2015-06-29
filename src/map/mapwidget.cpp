/* 
 * File:   mapwidget.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 15, 2015, 2:36 PM
 */

#include "mapwidget.h"

#include <QGridLayout>

#include "qfi/ui/LayoutSquare.h"
#include "qt-google-maps/mapsettings.h"
#include "core/hddsettings.h"
#include "core/mapconsts.h"
#include "mapview.h"
#include "mapoverlay.h"

MapWidget::MapWidget(HDDSettings* _hddSettings, MapSettings* _mapSettings, QWidget* _parent)
: QWidget(_parent),
  hddSettings(_hddSettings),
  mapSettings(_mapSettings)
{
//   LayoutSquare* layout = new LayoutSquare(this);
   QGridLayout* layout = new QGridLayout(this);
   layout->setContentsMargins(0, 0, 0, 0);
   
   QSizePolicy sp(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
   sp.setHeightForWidth(true);
   setSizePolicy(sp);
   
   
   view = new MapView(hddSettings, mapSettings);
   layout->addWidget(view);
   view->setSizePolicy(sp);
   
   overlay = new MapOverlay(hddSettings, mapSettings, view);
   overlay->setGeometry(view->geometry());
//   overlay->setSizePolicy(sp);
   
//   setLayout(layout);
   setMinimumSize(QSize(DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT));
   resize(DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT);
   qDebug() << "MapWidget size:" << size();
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
}

void MapWidget::setOrientation(MapOrientation mo)
{
   // The MapController::setOrientation() function directly updates the settings,
   // which the overlay uses.  But the MapView class still needs to change the
   // way it follows the aircraft.
   view->setOrientation(mo);
}

