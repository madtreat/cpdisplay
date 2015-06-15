/* 
 * File:   mapwidget.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 15, 2015, 11:19 AM
 */

#include "mapwidget.h"
#include <QWebView>
#include <QWebFrame>
#include <QWebElement>
#include <QMessageBox>

#include "geocode_data_manager.h"
#include "mapsettings.h"


MapWidget::MapWidget(MapSettings* _settings, QWidget* _parent)
: QWidget(_parent),
  settings(_settings)
{
   geocode = new GeocodeDataManager(settings->apiKey(), this);
   connect(geocode, SIGNAL(coordinatesReady(double,double)),  this, SLOT(showCoordinates(double,double)));
   connect(geocode, SIGNAL(errorOccurred(QString)),           this, SLOT(errorOccurred(QString)));
   
   QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true);
   mapView = new QWebView(this);
   mapView->setUrl(QUrl::fromLocalFile(settings->mapHtmlPath()));
   
   if (settings->canEnableMaps()) {
      enabled = true;
   }
   
   setMinimumSize(QSize(640, 480));
}

//MapWidget::MapWidget(const MapWidget& orig)
//{
//}

MapWidget::~MapWidget()
{
}


void MapWidget::showCoordinates(double lat, double lon, bool saveMarker)
{
   qDebug() << "Form, showCoordinates" << lat << "," << lon;
   
   QString str =
           QString("var newLoc = new google.maps.LatLng(%1, %2); ").arg(lat).arg(lon) +
           QString("map.setCenter(newLoc);") +
           QString("map.setZoom(%1);").arg(currentZoom);
   
   qDebug() << str;
   
   mapView->page()->currentFrame()->documentElement().evaluateJavaScript(str);
   
//   if (saveMarker)
//      setMarker(lat, lon, ui->lePostalAddress->text());
}

void MapWidget::errorOccurred(const QString& error)
{
   QMessageBox::warning(this, tr("Geocode Error"), error);
}

void MapWidget::updateZoomLevel(int level)
{
   currentZoom = level;
   QString str = QString("map.setZoom(%1);").arg(level);
   mapView->page()->currentFrame()->documentElement().evaluateJavaScript(str);
}

void MapWidget::incrementZoom()
{
   currentZoom++;
   updateZoomLevel(currentZoom);
}

void MapWidget::decrementZoom()
{
   currentZoom--;
   updateZoomLevel(currentZoom);
}

void MapWidget::updateOrientation(MapOrientation mo)
{
   // TODO: figure out how to change this
}