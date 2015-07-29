#include "switchboard.h"

#include <netinet/in.h>
#include <bitset>
#include <iostream>
#include <unistd.h>

#include <QUdpSocket>
#include <QByteArray>
#include <QDebug>

#include "hddsettings.h"

SwitchBoard::SwitchBoard(HDDSettings* _settings, QObject* _parent)
: QObject(_parent)
{
   settings = _settings;
   initSocket();

   requestDatarefsFromXPlane();
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

/*
 * Sends the RREF message to XPlane to request all necessary datarefs.
 */
#define DRMAP_INSERT(INDEX, STR, SIGNAL, FREQ) \
   drmap.insert(INDEX, new DRefValue(XPDR_OFFSET+INDEX, STR, &SwitchBoard::SIGNAL, FREQ));
void SwitchBoard::requestDatarefsFromXPlane()
{
   //DRMAP_INSERT(AC_TYPE,         XPDR_AC_TYPE,          acTypeUpdate);
   DRMAP_INSERT(AC_TAIL_NUM,     XPDR_AC_TAIL_NUM,      acTailNumUpdate,      1);
   DRMAP_INSERT(AC_NUM_ENGINES,  XPDR_AC_NUM_ENGINES,   acNumEnginesUpdate,   1);
   DRMAP_INSERT(RAD_COM1_FREQ,   XPDR_RADIO_COM1_FREQ,  radioCom1FreqUpdate,  2);
   DRMAP_INSERT(RAD_COM1_STDBY,  XPDR_RADIO_COM1_STDBY, radioCom1StdbyUpdate, 2);
   DRMAP_INSERT(RAD_COM2_FREQ,   XPDR_RADIO_COM2_FREQ,  radioCom2FreqUpdate,  2);
   DRMAP_INSERT(RAD_COM2_STDBY,  XPDR_RADIO_COM2_STDBY, radioCom2StdbyUpdate, 2);
   DRMAP_INSERT(RAD_NAV1_FREQ,   XPDR_RADIO_NAV1_FREQ,  radioNav1FreqUpdate,  2);
   DRMAP_INSERT(RAD_NAV1_STDBY,  XPDR_RADIO_NAV1_STDBY, radioNav1StdbyUpdate, 2);
   DRMAP_INSERT(RAD_NAV2_FREQ,   XPDR_RADIO_NAV2_FREQ,  radioNav2FreqUpdate,  2);
   DRMAP_INSERT(RAD_NAV2_STDBY,  XPDR_RADIO_NAV2_STDBY, radioNav2StdbyUpdate, 2);

   foreach (XPDataIndex i, drmap.keys()) {
      DRefValue* val = drmap.value(i);
      qDebug() << "Dataref" << i << "(" << val->xpIndex << ") @" << val->freq << "hz:" << drmap.value(i)->str;

      xp_dref_in dref;
      dref.freq = (xpint) val->freq;
      dref.code = (xpint) val->xpIndex;
      memset(&dref.data, 0, sizeof(dref.data));
      memcpy(&dref.data, val->str, QString(val->str).size());
      
      const int len = ID_DIM + sizeof(xp_dref_in);
      char* data = new char[len]();
      memset(data, 0, len);
      memcpy(data, RREF_PREFIX, ID_DIM);
      memcpy(data + ID_DIM, &dref, sizeof(xp_dref_in));
      
      xplane->writeDatagram(data, len, settings->xplaneHost(), 49000);
      delete data;

      // sleep to ensure the packets do not bunch up
      //usleep(3000000);
   }
}

void SwitchBoard::processDatagram(QByteArray& data)
{
   // Remove the first 5 bytes to get the raw values
   QByteArray header = data.mid(0, 5);
   QByteArray values = data.remove(0, 5);

   xp_dref_out* dref = (struct xp_dref_out*) values.data();

   qDebug() << "data received:" << header << dref->code << dref->data;

   notifyAll(dref);
   
   // XPlane < 10.40:

   // Each raw value is 36 bytes: 4 bytes=index from X-Plane, 32 bytes of data
   /*
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
   // */
}

/*
 * XPlane 10.40+ version
 * 
 * Notify everyone of new data.  This parses the data's values and emits signals
 * that other objects can be connected to.
 */
void SwitchBoard::notifyAll(xp_dref_out* dref)
{
   DRefValue* val = drmap.value((XPDataIndex) (dref->code - XPDR_OFFSET));
   if (val) {
      func_pointer signal = val->signal;
      emit (this->*(val->signal))(dref->data);
   }
   else {
      qWarning() << "Warning: invalid data from xplane";
   }
}

/*
 * XPlane < 10.40 version
 * 
 * Notify everyone of new data.  This parses the data's values and emits signals
 * that other objects can be connected to.
 */
void SwitchBoard::notifyAll(XPOutputData* data)
{
   #define VALUE(pos) data->values.at(pos).toFloat()
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
