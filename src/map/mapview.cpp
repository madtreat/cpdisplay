/* 
 * File:   mapview.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 15, 2015, 11:19 AM
 */

#include "mapview.h"
#include <QWebView>
#include <QWebFrame>
#include <QWebElement>
#include <QMessageBox>

#include "geocode_data_manager.h"
#include "mapsettings.h"


MapView::MapView(MapSettings* _settings, QWidget* _parent)
: QWidget(_parent),
  settings(_settings)
{
   geocode = new GeocodeDataManager(settings->apiKey(), this);
   connect(geocode, SIGNAL(coordinatesReady(double,double)),  this, SLOT(showCoordinates(double,double)));
   connect(geocode, SIGNAL(errorOccurred(QString)),           this, SLOT(errorOccurred(QString)));
   
   QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true);
   webView = new QWebView(this);
   webView->setUrl(QUrl::fromLocalFile(settings->mapHtmlPath()));
   
   if (settings->canEnableMaps()) {
      enabled = true;
   }
   setMinimumSize(QSize(640, 640));
}

//MapView::MapView(const MapView& orig)
//{
//}

MapView::~MapView()
{
}


void MapView::showCoordinates(double lat, double lon, bool saveMarker)
{
   qDebug() << "Form, showCoordinates" << lat << "," << lon;
   
   QString str =
           QString("var newLoc = new google.maps.LatLng(%1, %2); ").arg(lat).arg(lon) +
           QString("map.setCenter(newLoc);") +
           QString("map.setZoom(%1);").arg(settings->zoom());
   
   qDebug() << str;
   
   webView->page()->currentFrame()->documentElement().evaluateJavaScript(str);
   
//   if (saveMarker)
//      setMarker(lat, lon, ui->lePostalAddress->text());
}

void MapView::errorOccurred(const QString& error)
{
   QMessageBox::warning(this, tr("Geocode Error"), error);
}

void MapView::setZoom(int level)
{
   QString str = QString("map.setZoom(%1);").arg(level);
   webView->page()->currentFrame()->documentElement().evaluateJavaScript(str);
}
