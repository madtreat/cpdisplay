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
#define VALUE(pos) data->values.at(pos).toFloat()
void SwitchBoard::notifyAll(XPData* data)
{
   switch (data->index) {
      case TIMES:
         emit timeUpdate(VALUE(5), VALUE(6));
         break;
         
      case SPEEDS:
         emit speedUpdate(VALUE(0));
         break;

      case MACH_VVI_GLOAD:
         emit machNumUpdate(VALUE(0));
         emit vertVelUpdate(VALUE(2));
         break;

      case SYS_PRESSURE:
         emit pressureUpdate(VALUE(0));
         break;

      case ANG_VEL:
         emit angVelUpdate(VALUE(0), VALUE(1), VALUE(2));
         break;

      case PITCH_ROLL_HEADINGS:
         emit pitchUpdate(VALUE(0));
         emit rollUpdate(VALUE(1));
         emit headingTrueUpdate(VALUE(2));
         emit headingMagUpdate(VALUE(3));
         break;

      case AOA_SIDESLIP_PATHS:
         emit aoaSideSlipUpdate(VALUE(0), VALUE(1));
         emit hPathUpdate(VALUE(2));
         emit vPathUpdate(VALUE(3));
         emit slipSkidUpdate(VALUE(7));
         break;

      case MAG_COMPASS:
         emit compassUpdate(VALUE(0));
         break;
         
      case LAT_LON_ALT:
         emit latLonUpdate(VALUE(0), VALUE(1));
         emit altMSLUpdate(VALUE(2));
         emit altAGLUpdate(VALUE(3));
         break;
         
      // For ALL_LAT, ALL_LON, ALL_ALT, the first value (at 0) is this aircraft
      case ALL_LAT:
         emit acLatUpdate(VALUE(1), 1);
         emit acLatUpdate(VALUE(2), 2);
         emit acLatUpdate(VALUE(3), 3);
         emit acLatUpdate(VALUE(4), 4);
         emit acLatUpdate(VALUE(5), 5);
         emit acLatUpdate(VALUE(6), 6);
         emit acLatUpdate(VALUE(7), 7);
         break;
         
      case ALL_LON:
         emit acLonUpdate(VALUE(1), 1);
         emit acLonUpdate(VALUE(2), 2);
         emit acLonUpdate(VALUE(3), 3);
         emit acLonUpdate(VALUE(4), 4);
         emit acLonUpdate(VALUE(5), 5);
         emit acLonUpdate(VALUE(6), 6);
         emit acLonUpdate(VALUE(7), 7);
         break;
         
      case ALL_ALT:
         emit acAltUpdate(VALUE(1), 1);
         emit acAltUpdate(VALUE(2), 2);
         emit acAltUpdate(VALUE(3), 3);
         emit acAltUpdate(VALUE(4), 4);
         emit acAltUpdate(VALUE(5), 5);
         emit acAltUpdate(VALUE(6), 6);
         emit acAltUpdate(VALUE(7), 7);
         break;
         
      case COM_1_2_FREQ:
         emit com1Update(VALUE(0), VALUE(1));
         emit com2Update(VALUE(3), VALUE(4));
         emit comTransmitUpdate(VALUE(6));
         break;
         
      case NAV_1_2_FREQ:
         emit nav1Update(VALUE(0), VALUE(1));
         emit nav2Update(VALUE(4), VALUE(5));
         break;
         
      default:
         break;
   }
   
}
