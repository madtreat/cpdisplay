#include "switchboard.h"

#include <netinet/in.h>
#include <bitset>
#include <iostream>
#include <QUdpSocket>
#include <QByteArray>
#include <QDebug>

#include "hddsettings.h"
#include "xplanedata.h"

SwitchBoard::SwitchBoard(HDDSettings* _settings, QObject* _parent)
: QObject(_parent)
{
   settings = _settings;
   initSocket();
}


SwitchBoard::~SwitchBoard()
{
}


void SwitchBoard::initSocket()
{
   xplane = new QUdpSocket(this);
   //xplane->bind(settings->xplaneHost(), settings->xplanePort());
   xplane->bind(settings->xplanePort(), QUdpSocket::ShareAddress);
   
   connect(xplane, SIGNAL(readyRead()), this, SLOT(readPendingData()));
}

void SwitchBoard::readPendingData()
{
   while (xplane->hasPendingDatagrams()) {
      QByteArray datagram;
      datagram.resize(xplane->pendingDatagramSize());
      QHostAddress sender;
      quint16 senderPort;
      
      xplane->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
      
      processDatagram(datagram);
   }
}

void SwitchBoard::processDatagram(QByteArray& data)
{
   // Remove the first 5 bytes to get the raw values
   QByteArray header = data.mid(0, 5);
   QByteArray values = data.remove(0, 5);
   
   // Each raw value is 36 bytes: 4 bytes=index from X-Plane, 32 bytes of data
   int numValues = values.size()/36;
//   qDebug() << "Processing datagram of size:" << data.size() << numValues;
   
   // Separate each value
   for (int i = 0; i < numValues; i++) {
      // Get the 36 bytes starting at i*36
      QByteArray valueBytes = values.mid(i*36, 36);
      XPData* data = new XPData();
      data->parseRawData(valueBytes);
      
      notifyAll(data);
   }
}

/*
 * Notify everyone of new data.  This parses the data's values and emits signals
 * that other objects can be connected to.
 */
void SwitchBoard::notifyAll(XPData* data)
{
   switch (data->index) {
      case SPEEDS:
         emit speedUpdate(data->values.at(0).toFloat());
         break;

      case MACH_VVI_GLOAD:
         emit machNumUpdate(data->values.at(0).toFloat());
         emit vertVelUpdate(data->values.at(2).toFloat());
         break;

      case SYS_PRESSURE:
         emit pressureUpdate(data->values.at(0).toFloat());
         break;

      case PITCH_ROLL_HEADINGS:
         emit pitchUpdate(data->values.at(0).toFloat());
         emit rollUpdate(data->values.at(1).toFloat());
         emit headingTrueUpdate(data->values.at(2).toFloat());
         emit headingMagUpdate(data->values.at(3).toFloat());
         break;

      case AOA_SIDESLIP_PATHS:
         emit aoaSideSlipUpdate(data->values.at(0).toFloat(), data->values.at(1).toFloat());
         emit hPathUpdate(data->values.at(2).toFloat());
         emit vPathUpdate(data->values.at(3).toFloat());
         emit slipSkidUpdate(data->values.at(7).toFloat());
         break;

      case MAG_COMPASS:
         emit compassUpdate(data->values.at(0).toFloat());
         break;
         
      case LAT_LON_ALT:
         emit latLonUpdate(data->values.at(0).toFloat(), data->values.at(1).toFloat(), 0);
         emit altMSLUpdate(data->values.at(2).toFloat());
         emit altAGLUpdate(data->values.at(3).toFloat());
         break;
         
      case ALL_LAT:
         break;
         
      case ALL_LON:
         break;
         
      case ALL_ALT:
         break;
         
      default:
         break;
   }
   
}
