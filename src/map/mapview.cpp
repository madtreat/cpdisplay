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

#include "qt-google-maps/geocode_data_manager.h"
#include "qt-google-maps/mapsettings.h"

#include "core/hddsettings.h"
#include "core/mapconsts.h"


MapView::MapView(HDDSettings* _hddSettings, MapSettings* _settings, QWidget* _parent)
: QWidget(_parent),
  hddSettings(_hddSettings),
  settings(_settings)
{
   geocode = new GeocodeDataManager(settings->apiKey(), this);
   connect(geocode, SIGNAL(coordinatesReady(double,double)),  this, SLOT(showCoordinates(double,double)));
   connect(geocode, SIGNAL(errorOccurred(QString)),           this, SLOT(errorOccurred(QString)));
   
   QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true);
   webView = new QWebView(this);
   connect(webView, SIGNAL(loadStarted()), this, SLOT(startedLoading()));
   connect(webView, SIGNAL(loadProgress(int)), this, SLOT(loadingProgress(int)));
   connect(webView, SIGNAL(loadFinished(bool)), this, SLOT(finishedLoading(bool)));
   
   qDebug() << "WebView loading HTML file from" << settings->mapHtmlPath();
   webView->setUrl(QUrl::fromLocalFile(settings->mapHtmlPath()));
   
   if (settings->canEnableMaps()) {
      enabled = true;
   }
   setMinimumSize(QSize(DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT));
}

//MapView::MapView(const MapView& orig)
//{
//}

MapView::~MapView()
{
}

QSize MapView::sizeHint() const
{
   return QSize(DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT);
}

void MapView::resize(int w, int h)
{
   resize(QSize(w, h));
}

void MapView::resize(const QSize& size)
{
   QWidget::resize(size);
   qDebug() << "Resizing MapView to" << size;
   // resize the map, keeping the current center
   QString str = "var center = map.getCenter();";
   str += "google.maps.event.trigger(map,\"'resize\");";
   str += "map.setCenter(center);";
   
//   QVariant response = evaluateJS(str);
//   qDebug() << "   JS response:" << response;
   
   // set the web page's size
   qDebug() << "   Current viewport size:" << webView->page()->viewportSize();
   qDebug() << "   Current webframe size:" << webView->page()->currentFrame()->contentsSize();
   webView->page()->setViewportSize(size);
   qDebug() << "       New viewport size:" << webView->page()->viewportSize();
}


//void MapView::paintEvent(QPaintEvent*)
//{
//   QPainter p(this);
//   int centerX = width()/2;
//   int centerY = height()/2;
//   
//   if (hddSettings->mapOrientation() == TRACK_UP) {
//      p.translate(centerX, centerY);
//      p.rotate(heading);
//   }
//}


QVariant MapView::evaluateJS(QString js)
{
   return webView->page()->currentFrame()->documentElement().evaluateJavaScript(js);
}

void MapView::startedLoading()
{
   qDebug() << "Started loading URL";
}

void MapView::loadingProgress(int percent)
{
//   qDebug() << "Loading progress:" << percent << "%";
}

void MapView::finishedLoading(bool success)
{
   qDebug() << "Finished loading URL, successful:" << success;
   resize(size());
}


void MapView::showCoordinates(double lat, double lon, bool saveMarker)
{
   qDebug() << "Form, showCoordinates" << lat << "," << lon;
   
   QString str = QString("var newLoc = new google.maps.LatLng(%1, %2); ").arg(lat).arg(lon);
   str += QString("map.setCenter(newLoc);");
   str += QString("map.setZoom(%1);").arg(settings->zoom());
   
   qDebug() << str;
   
   evaluateJS(str);
   
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
   evaluateJS(str);
   calculateDistanceScale();
}

void MapView::panToLocation(float lat, float lon)
{
   QString str = QString("map.panTo(new google.maps.LatLng(%1, %2), 500);").arg(lat).arg(lon);
   evaluateJS(str);
}

void MapView::calculateDistanceScale()
{
   qDebug() << "Calculating distance scale...";
   QString latlon1 = QString("map.getBounds().getNortEast()");
   QString latlon2 = QString("map.getBounds().getSouthWest()");

   QString str = QString("google.maps.geometry.spherical.computeDistanceBetween (%1, %2);").arg(latlon1).arg(latlon2);
   QVariant diagDist = evaluateJS(str);
   qDebug() << "diagDist =" << diagDist;
}
