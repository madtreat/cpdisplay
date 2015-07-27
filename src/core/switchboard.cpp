#include "switchboard.h"

#include <netinet/in.h>
#include <bitset>
#include <iostream>
#include <QUdpSocket>
#include <QByteArray>
#include <QDebug>

#include "hddsettings.h"
#include "xplanedata.h"
#include "xplanedref.h"

SwitchBoard::SwitchBoard(HDDSettings* _settings, QObject* _parent)
: QObject(_parent)
{
   settings = _settings;
   initSockets();

   xp_dref_in dref;
   dref.freq = 1;
   dref.code = 1011;
   //dref.data = XPDR_AC_NUM_ENG;
   memset(&dref.data, 0, sizeof(dref.data));
   memcpy(&dref.data, XPDR_AC_NUM_ENG, QString(XPDR_AC_NUM_ENG).size());

   const int len = ID_DIM + sizeof(xp_dref_in);
   char* data = new char[len]();//[len];
   memset(data, 0, len);
   memcpy(data, RREF_PREFIX, ID_DIM);
   memcpy(&data[ID_DIM], &dref, sizeof(xp_dref_in));

   xplane->writeDatagram(data, len, settings->xplaneHost(), 49000);
qDebug() << "Send test datagram to xplane host:" << settings->xplaneHost();
}


SwitchBoard::~SwitchBoard()
{
}


void SwitchBoard::initSockets()
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

   xp_dref_out* dref = (struct xp_dref_out*) values.data();

   qDebug() << "data received:" << header << dref->code << dref->data;
   
   // Each raw value is 36 bytes: 4 bytes=index from X-Plane, 32 bytes of data
   int numValues = values.size()/36;
//   qDebug() << "Processing datagram of size:" << data.size() << numValues;
   
   // Separate each value
   for (int i = 0; i < numValues; i++) {
      // Get the 36 bytes starting at i*36
      QByteArray valueBytes = values.mid(i*36, 36);
      XPOutputData* data = new XPOutputData();
      data->parseRawData(valueBytes);
      
      notifyAll(data);
   }
}

/*
 * Notify everyone of new data.  This parses the data's values and emits signals
 * that other objects can be connected to.
 */
#define VALUE(pos) data->values.at(pos).toFloat()
void SwitchBoard::notifyAll(XPOutputData* data)
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
         for (int i = 0; i < 8; i++) {
            if (VALUE(i) != 0.0)
               emit acLatUpdate(VALUE(i), i);
         }
         break;
         
      case ALL_LON:
         for (int i = 0; i < 8; i++) {
            if (VALUE(i) != 0.0)
               emit acLonUpdate(VALUE(i), i);
         }
         break;
         
      case ALL_ALT:
         for (int i = 0; i < 8; i++) {
            if (VALUE(i) != 0.0)
               emit acAltUpdate(VALUE(i), i);
         }
         break;
         
      case THROTTLE_COMMAND:
         for (int i = 0; i < 8; i++) {
            if (VALUE(i) != 0.0)
               emit throttleCommandUpdate(VALUE(i), i);
         }
         break;
         
      case THROTTLE_ACTUAL:
         for (int i = 0; i < 8; i++) {
            if (VALUE(i) != 0.0)
               emit throttleActualUpdate(VALUE(i), i);
         }
         break;
      
      case ENG_POWER:
         for (int i = 0; i < 8; i++) {
            if (VALUE(i) != 0.0)
               emit engPowerUpdate(VALUE(i), i);
         }
         break;
      
      case ENG_THRUST:
         for (int i = 0; i < 8; i++) {
            if (VALUE(i) != 0.0)
               emit engThrustUpdate(VALUE(i), i);
         }
         break;
      
      case ENG_TORQUE:
         for (int i = 0; i < 8; i++) {
            if (VALUE(i) != 0.0)
               emit engTorqueUpdate(VALUE(i), i);
         }
         break;
      
      case ENG_RPM:
         for (int i = 0; i < 8; i++) {
            if (VALUE(i) != 0.0)
               emit engRPMUpdate(VALUE(i), i);
         }
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
         qDebug() << "Unknown data receievd:" << data->index << VALUE(0);
         break;
   }
   
}
