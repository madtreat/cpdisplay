#include "switchboard.h"

#include <netinet/in.h>
#include <bitset>
#include <iostream>
#include <unistd.h>
#include <string.h>

#include <QUdpSocket>
#include <QByteArray>
#include <QList>
#include <QDebug>

#include "cpdsettings.h"

#define DEBUG_SEND 1
#define DEBUG_RECV 0


SwitchBoard::SwitchBoard(CPDSettings* _settings, QObject* _parent)
: QObject(_parent),
  drefID(NUM_DATA_INDEXES)
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
   //xplane->bind(settings->xplaneHost(), settings->xplanePort(), QUdpSocket::ShareAddress);
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
      //qDebug() << "Packet size" << datagram.size() << "  \tfrom" << sender << ":" << senderPort;
      
      processDatagram(datagram);
   }
}

void SwitchBoard::sendDREF(QString drefStr, xpflt value)
{
   if (DEBUG_SEND)
      qDebug() << "Sending DREF packet:" << drefStr << "(" << value << ")";
   xp_dref_in dref;
   dref.value = value;
   memset(&dref.dref_path, 0, sizeof(dref.dref_path));
   memcpy(&dref.dref_path, drefStr.toLocal8Bit().data(), drefStr.size());
   
   const int len = ID_DIM + sizeof(xp_dref_in);
   char data[len];
   memset(&data, 0, len);
   memcpy(&data, DREF_PREFIX, ID_DIM);
   memcpy(&data[ID_DIM], &dref, sizeof(xp_dref_in));
   
   xplane->writeDatagram(data, len, settings->xplaneHost(), settings->xplanePort());
}


/*
 * Sends the RREF message to XPlane to request all necessary datarefs.
 * 
 * The DRMAP_INSERT_D and DRMAP_INSERT_N macros simplify the very repetitive
 * task of creating a new DRefValue objects and inserting them into the drmap.
 * DRMAP_INSERT_D will insert an object with a direct_fp signal, where
 * DRMAP_INSERT_N will insert an object with a numbered_fp signal.
 *
 * The arguments are:
 *    STR            = the dataref string used by xplane
 *    SIG_DIRECT     = the signal function for direct value updates
 *    SIG_NUMBERED   = the signal function for numbered value updates
 *    FREQ           = the frequency with which xplane will emit updates
 *    SIG_NUM        = the number used in a SIG_NUMBERED signal
 *                      example: gear2 update SIG_NUM is 2
 *
 * NOTES:
 *  - The dataref's ID is determined programmatically so you do not have
 *    to keep track of ID's for every single dataref you request.
 *  - The curly braces in each of the macros are used to keep "int id = ..."
 *    from re-declaring the id variable each time the macro is called, which
 *    would throw compiler errors.
 */
void SwitchBoard::requestDatarefsFromXPlane()
{
   #define DRMAP_INSERT_D(STR, SIG_DIRECT, FREQ) \
   { \
      int id = nextDRefID(); \
      drmap.insert(id, new DRefValue(id, STR, &SwitchBoard::SIG_DIRECT, NULL, FREQ)); \
   }
   #define DRMAP_INSERT_N(STR, SIG_NUMBERED, FREQ, SIG_NUM) \
   { \
      int id = nextDRefID(); \
      drmap.insert(id, new DRefValue(id, STR, NULL, &SwitchBoard::SIG_NUMBERED, FREQ, SIG_NUM)); \
   }
   
   // Request datarefs from xplane (does not work in < 10.40b7: known bug:
   // http://forums.x-plane.org/index.php?showtopic=87772)

   //DRMAP_INSERT_D(XPDR_AC_TYPE,           acTypeUpdate,           1);
   DRMAP_INSERT_D(XPDR_AC_TAIL_NUM_1,     acTailNumUpdate,        1);
   DRMAP_INSERT_D(XPDR_AC_NUM_ENGINES,    acNumEnginesUpdate,     1);

   DRMAP_INSERT_D(XPDR_RADIO_COM1_FREQ,   radioCom1FreqUpdate,    2);
   DRMAP_INSERT_D(XPDR_RADIO_COM1_STDBY,  radioCom1StdbyUpdate,   2);
   DRMAP_INSERT_D(XPDR_RADIO_COM2_FREQ,   radioCom2FreqUpdate,    2);
   DRMAP_INSERT_D(XPDR_RADIO_COM2_STDBY,  radioCom2StdbyUpdate,   2);
   DRMAP_INSERT_D(XPDR_RADIO_NAV1_FREQ,   radioNav1FreqUpdate,    2);
   DRMAP_INSERT_D(XPDR_RADIO_NAV1_STDBY,  radioNav1StdbyUpdate,   2);
   DRMAP_INSERT_D(XPDR_RADIO_NAV2_FREQ,   radioNav2FreqUpdate,    2);
   DRMAP_INSERT_D(XPDR_RADIO_NAV2_STDBY,  radioNav2StdbyUpdate,   2);

   for (int i = 0; i < MAX_NUM_FUEL_TANKS; i++) {
      QString vstr = XPDR_CP_FUEL_QTY_X;
      vstr.replace("__X__", QString::number(i));
      DRMAP_INSERT_N(vstr, fuelQuantityUpdate, 4, i);
   }

   for (int i = 0; i < MAX_NUM_LANDING_GEARS; i++) {
      QString vstr = XPDR_GEAR_DEPLOY_X;
      vstr.replace("__X__", QString::number(i));
      DRMAP_INSERT_N(vstr, gearDeployUpdate, 2, i);
   }

   foreach (int i, drmap.keys()) {
      DRefValue* val = drmap.value(i);
      QString vstr = val->str;
      // qDebug() << "Dataref" << i << "(" << val->xpIndex << ") @" << val->freq << "hz:" << vstr;

      xp_rref_in dref;
      dref.freq = (xpint) val->freq;
      dref.code = (xpint) val->xpIndex;
      memset(&dref.data, 0, sizeof(dref.data));
      memcpy(&dref.data, vstr.toLocal8Bit().data(), vstr.size());
      
      const int len = ID_DIM + sizeof(xp_rref_in);
      char data[len];
      memset(&data, 0, len);
      memcpy(&data, RREF_PREFIX, ID_DIM);
      memcpy(&data[ID_DIM], &dref, sizeof(xp_rref_in));
      
      xplane->writeDatagram(data, len, settings->xplaneHost(), settings->xplanePort());
   }


   // Turn on UDP output
   // This works (the DSEL packet to xplane), at least in 10.40b7
   QList<XPDataIndex> indexes;
   indexes.append((XPDataIndex) 1);
   indexes.append((XPDataIndex) 3);
   indexes.append((XPDataIndex) 4);

   indexes.append((XPDataIndex) 15);
   indexes.append((XPDataIndex) 16);
   indexes.append((XPDataIndex) 17);
   indexes.append((XPDataIndex) 18);
   indexes.append((XPDataIndex) 19);
   indexes.append((XPDataIndex) 20);
   indexes.append((XPDataIndex) 21);
   indexes.append((XPDataIndex) 22);
   indexes.append((XPDataIndex) 23);
   indexes.append((XPDataIndex) 24);
   indexes.append((XPDataIndex) 25);
   indexes.append((XPDataIndex) 26);
   
   indexes.append((XPDataIndex) 34);
   indexes.append((XPDataIndex) 35);
   indexes.append((XPDataIndex) 36);
   indexes.append((XPDataIndex) 37);
   indexes.append((XPDataIndex) 38);
   indexes.append((XPDataIndex) 39);
   indexes.append((XPDataIndex) 40);
   indexes.append((XPDataIndex) 41);
   indexes.append((XPDataIndex) 42);
   indexes.append((XPDataIndex) 43);
   indexes.append((XPDataIndex) 44);
   indexes.append((XPDataIndex) 45);
   indexes.append((XPDataIndex) 46);
   indexes.append((XPDataIndex) 47);
   indexes.append((XPDataIndex) 48);
   indexes.append((XPDataIndex) 49);
   indexes.append((XPDataIndex) 50);

   indexes.append((XPDataIndex) 96);
   indexes.append((XPDataIndex) 97);


   // DSEL_PREFIX + i*cs + 3*0 for padding
   // == DSEL0 + ___ + 0
   int cs = sizeof(xpint);
   const int len2 = ID_DIM+3 + cs*indexes.size(); // +3 to pad: len%4==0
   const int len3 = (len2%4 == 0) ? len2 : len2 + len2%4;
   char dsel[len3];
   memset(&dsel, 0, len3);
   memcpy(&dsel, DSEL_PREFIX, ID_DIM);

   for (int i = 0; i < indexes.size(); i++) {
      memset(&dsel[ID_DIM+(i*cs)], (xpint) indexes.at(i), cs);
   }
   xplane->writeDatagram(dsel, len2, settings->xplaneHost(), settings->xplanePort());
}

void SwitchBoard::processDatagram(QByteArray& data)
{
   // Remove the first 5 bytes to get the raw values
   QByteArray header = data.mid(0, ID_DIM);
   QByteArray values = data.remove(0, ID_DIM);

   // XPlane >= 10.40b7:

   // Note the RREFO ends with a capitol O, not the number zero (0)
   if (header == "RREFO") {
      int size = sizeof(xp_rref_out);
      int numValues = values.size()/size;
      if (DEBUG_RECV)
         qDebug() << "Received RREFO with" << numValues << "values:";

      for (int i = 0; i < numValues; i++) {
         xp_rref_out* dref = (struct xp_rref_out*) values.mid(i*size, size).data();
         xpint code  = dref->code;
         xpflt value = dref->data;
         /*
          * Somehow, calling dref->code after dref->data turns the value at 
          * dref->code into some erroneous data, so variables store the code
          * and value directly, as soon as the dref object is constructed.
          */
         if (DEBUG_RECV)
            qDebug() << "   data received:" << header << dref->code << dref->data;

         notifyAll((int) code, value);
      }
      if (DEBUG_RECV)
         qDebug() << "--- --- --- --- ---";
   }


   // XPlane < 10.40, or raw UDP output:

   if (header == "DATA@") {
      // Each raw value is 36 bytes: 4 bytes=index from X-Plane, 32 bytes of data
      int size = 36;
      int numValues = values.size()/size;
      // qDebug() << "Received DATA@ with" << numValues << "values:";
      
      // Separate each value
      for (int i = 0; i < numValues; i++) {
         // Get the size bytes starting at i*size
         QByteArray valueBytes = values.mid(i*size, size);
         XPOutputData* outData = new XPOutputData();
         outData->parseRawData(valueBytes);
         
         notifyAll(outData);
      }
   }
}


/*
 * XPlane 10.40+ version
 * 
 * Notify everyone of new data.  This parses the data's values and emits signals
 * that other objects can be connected to.
 */
void SwitchBoard::notifyAll(int code, xpflt value)
{
   DRefValue* val = drmap.value(code);
   if (val) {
      direct_fp   sigDirect   = val->signalDirect;
      numbered_fp sigNumbered = val->signalNumbered;
      if (sigDirect) {
         emit (this->*(sigDirect))(value);
      }
      if (sigNumbered) {
         emit (this->*(sigNumbered))(value, val->signalNum);
      }
   }
   else {
      qWarning() << "Warning: invalid data from xplane";
   }
}


/*
 * XPlane < 10.40 / raw UDP output version
 * 
 * Notify everyone of new data.  This parses the data's values and emits signals
 * that other objects can be connected to.
 */
void SwitchBoard::notifyAll(XPOutputData* data)
{
   #define VALUE(pos) data->values.at(pos).toFloat()
   switch (data->index) {
      case TIMES:
         emit timeUpdate(VALUE(5), VALUE(6), VALUE(2), VALUE(3));
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
      
      case PROP_RPM:
         for (int i = 0; i < 8; i++) {
            if (VALUE(i) != 0.0)
               emit propRPMUpdate(VALUE(i), i);
         }
         break;
      
      case PROP_PITCH:
         for (int i = 0; i < 8; i++) {
            if (VALUE(i) != 0.0)
               emit propPitchUpdate(VALUE(i), i);
         }
         break;
      
      case PROPWASH:
         for (int i = 0; i < 8; i++) {
            if (VALUE(i) != 0.0)
               emit propwashUpdate(VALUE(i), i);
         }
         break;
      
      case N1:
         for (int i = 0; i < 8; i++) {
            if (VALUE(i) != 0.0)
               emit n1Update(VALUE(i), i);
         }
         break;
      
      case N2:
         for (int i = 0; i < 8; i++) {
            if (VALUE(i) != 0.0)
               emit n2Update(VALUE(i), i);
         }
         break;
      
      case MP:
         for (int i = 0; i < 8; i++) {
            if (VALUE(i) != 0.0)
               emit mpUpdate(VALUE(i), i);
         }
         break;
      
      case EPR:
         for (int i = 0; i < 8; i++) {
            if (VALUE(i) != 0.0)
               emit eprUpdate(VALUE(i), i);
         }
         break;
      
      case FF:
         for (int i = 0; i < 8; i++) {
            if (VALUE(i) != 0.0)
               emit ffUpdate(VALUE(i), i);
         }
         break;
      
      case ITT:
         for (int i = 0; i < 8; i++) {
            if (VALUE(i) != 0.0)
               emit ittUpdate(VALUE(i), i);
         }
         break;
      
      case EGT:
         for (int i = 0; i < 8; i++) {
            if (VALUE(i) != 0.0)
               emit egtUpdate(VALUE(i), i);
         }
         break;
      
      case CHT:
         for (int i = 0; i < 8; i++) {
            if (VALUE(i) != 0.0)
               emit chtUpdate(VALUE(i), i);
         }
         break;
      
      case OIL_PRESSURE:
         for (int i = 0; i < 8; i++) {
            if (VALUE(i) != 0.0)
               emit engOilPressureUpdate(VALUE(i), i);
         }
         break;
      
      case OIL_TEMP:
         for (int i = 0; i < 8; i++) {
            if (VALUE(i) != 0.0)
               emit engOilTempUpdate(VALUE(i), i);
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
         //qDebug() << "Unknown data receievd:" << data->index << VALUE(0);
         break;
   }
}

void SwitchBoard::setDataref(QString dataref, float value)
{
   // TODO: send xp_dref_in message to xplane
}
