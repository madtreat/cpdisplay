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
#include <QTimer>

#include "cpdsettings.h"

#define DEBUG_SEND 1
#define DEBUG_RECV_UDP 0
#define DEBUG_RECV_RREF 0
#define DEBUG_RECV (DEBUG_RECV_UDP | DEBUG_RECV_RREF)


// Direct-signal-only constructor
SwitchBoard::DRefValue::DRefValue(int _index, QString _str, int _freq, direct_fp _fn_d) {
   xpIndex = _index;
   str = _str;
   signalDirect = _fn_d;
   signalNumbered = NULL;
   signalLimit = NULL;
   freq = _freq;
   signalNum = -1;
   limitType = NO_LIMIT;
}

// Numbered-signal-only constructor
SwitchBoard::DRefValue::DRefValue(int _index, QString _str, int _freq, numbered_fp _fn_n, int _sigNum) {
   xpIndex = _index;
   str = _str;
   signalDirect = NULL;
   signalNumbered = _fn_n;
   signalLimit = NULL;
   freq = _freq;
   signalNum = _sigNum;
   limitType = NO_LIMIT;
}

// Limit-signal-only constructor
SwitchBoard::DRefValue::DRefValue(int _index, QString _str, int _freq, limit_fp _fn_l, LimitType _limitType) {
   xpIndex = _index;
   str = _str;
   signalDirect = NULL;
   signalNumbered = NULL;
   signalLimit = _fn_l;
   freq = _freq;
   signalNum = -1;
   limitType = _limitType;
}

// All-signals constructor: can use NULL to select only desired signal types
SwitchBoard::DRefValue::DRefValue(int _index,
          QString _str,
          int _freq,
          direct_fp _fn_d,
          numbered_fp _fn_n,
          limit_fp _fn_l,
          int _sigNum,
          LimitType _limitType) {
   xpIndex = _index;
   str = _str;
   signalDirect = _fn_d;
   signalNumbered = _fn_n;
   signalLimit = _fn_l;
   freq = _freq;
   signalNum = _sigNum;
   limitType = _limitType;
}

SwitchBoard::DRefValue::DRefValue(const DRefValue& rhs) {
   xpIndex = rhs.xpIndex;
   str = rhs.str;
   signalDirect = rhs.signalDirect;
   signalNumbered = rhs.signalNumbered;
   signalLimit = rhs.signalLimit;
   freq = rhs.freq;
   signalNum = rhs.signalNum;
   limitType = rhs.limitType;
}


SwitchBoard::SwitchBoard(CPDSettings* _settings, QObject* _parent)
: QObject(_parent),
  drefID(NUM_DATA_INDEXES)
{
   settings = _settings;

   xplane = new QUdpSocket(this);
   // TODO: figure out the differences between these two functions
   xplane->bind(settings->xplaneHost(), settings->xplanePortOut(), QUdpSocket::ShareAddress);
   //xplane->bind(settings->xplanePortOut(), QUdpSocket::ShareAddress);

   requestDatarefsFromXPlane();
   connect(xplane, SIGNAL(readyRead()), this, SLOT(readPendingData()));
}


SwitchBoard::~SwitchBoard()
{
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
   
   xplane->writeDatagram(data, len, settings->xplaneHost(), settings->xplanePortIn());
}


void SwitchBoard::addDirectDRef(QString str, int freq, direct_fp sig)
{
   int id = nextDRefID();
   DRefValue* val = new DRefValue(id, str, freq, sig);
   drmap.insert(id, val);
}

void SwitchBoard::addNumberedDRef(QString str, int freq, numbered_fp sig, int sigNum)
{
   int id = nextDRefID();
   DRefValue* val = new DRefValue(id, str, freq, sig, sigNum);
   drmap.insert(id, val);
}

void SwitchBoard::addLimitDRefHelp(QString str, int freq, limit_fp sig, LimitType lt)
{
   int id = nextDRefID();
   DRefValue* val = new DRefValue(id, str, freq, sig, lt);
   drmap.insert(id, val);
}

void SwitchBoard::addLimitDRef(QString str, int freq, limit_fp sig)
{
   addLimitDRefHelp(QString(str).replace("__LT__", "green_lo"),  freq, sig, LIMIT_G_LO);
   addLimitDRefHelp(QString(str).replace("__LT__", "green_hi"),  freq, sig, LIMIT_G_HI);
   addLimitDRefHelp(QString(str).replace("__LT__", "yellow_lo"), freq, sig, LIMIT_Y_LO);
   addLimitDRefHelp(QString(str).replace("__LT__", "yellow_hi"), freq, sig, LIMIT_Y_HI);
   addLimitDRefHelp(QString(str).replace("__LT__", "red_lo"),    freq, sig, LIMIT_R_LO);
   addLimitDRefHelp(QString(str).replace("__LT__", "red_hi"),    freq, sig, LIMIT_R_HI);
}


/*
 * Sends the RREF message to XPlane to request all necessary datarefs.
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
   // We do not want to receive our own packets, if this CPDisplay is running
   // on the same machine that xplane is running on.
   // xplane->blockSignals(true);

   // Request datarefs from xplane (does not work in < 10.40b7: known bug:
   // http://forums.x-plane.org/index.php?showtopic=87772)

   // Aircraft type and info
   //addDirectDRef(XPDR_AC_TYPE,           1, &SWB::acTypeUpdate);
   addDirectDRef(XPDR_AC_TAIL_NUM_1,     1, &SWB::acTailNumUpdate);
   addDirectDRef(XPDR_AC_NUM_ENGINES,    1, &SWB::acNumEnginesUpdate);
   addDirectDRef(XPDR_GEAR_RETRACTABLE,  1, &SWB::gearRetractableUpdate);

   // Radios
   addDirectDRef(XPDR_RADIO_COM1_FREQ,   2, &SWB::radioCom1FreqUpdate);
   addDirectDRef(XPDR_RADIO_COM1_STDBY,  2, &SWB::radioCom1StdbyUpdate);
   addDirectDRef(XPDR_RADIO_COM2_FREQ,   2, &SWB::radioCom2FreqUpdate);
   addDirectDRef(XPDR_RADIO_COM2_STDBY,  2, &SWB::radioCom2StdbyUpdate);
   addDirectDRef(XPDR_RADIO_NAV1_FREQ,   2, &SWB::radioNav1FreqUpdate);
   addDirectDRef(XPDR_RADIO_NAV1_STDBY,  2, &SWB::radioNav1StdbyUpdate);
   addDirectDRef(XPDR_RADIO_NAV2_FREQ,   2, &SWB::radioNav2FreqUpdate);
   addDirectDRef(XPDR_RADIO_NAV2_STDBY,  2, &SWB::radioNav2StdbyUpdate);

   // Fuel
   for (int i = 0; i < MAX_NUM_FUEL_TANKS; i++) {
      QString vstr = XPDR_CP_FUEL_QTY_X;
      vstr.replace("__X__", QString::number(i));
      addNumberedDRef(vstr, 4, &SWB::fuelQuantityUpdate, i);
   }

   // Gear
   for (int i = 0; i < MAX_NUM_LANDING_GEARS; i++) {
      QString vstr = XPDR_GEAR_DEPLOY_X;
      vstr.replace("__X__", QString::number(i));
      addNumberedDRef(vstr, 4, &SWB::gearDeployUpdate, i);
   }

   // Engine limits
   addLimitDRef(XPDR_ENG_LIMIT_MP,     1, &SWB::engLimitMPUpdate);
   addLimitDRef(XPDR_ENG_LIMIT_FF,     1, &SWB::engLimitFFUpdate);
   addLimitDRef(XPDR_ENG_LIMIT_N1,     1, &SWB::engLimitN1Update);
   addLimitDRef(XPDR_ENG_LIMIT_N2,     1, &SWB::engLimitN2Update);
 
   addLimitDRef(XPDR_ENG_LIMIT_EPR,    1, &SWB::engLimitEPRUpdate);
   addLimitDRef(XPDR_ENG_LIMIT_EGT,    1, &SWB::engLimitEGTUpdate);
   addLimitDRef(XPDR_ENG_LIMIT_TRQ,    1, &SWB::engLimitTRQUpdate);
   addLimitDRef(XPDR_ENG_LIMIT_ITT,    1, &SWB::engLimitITTUpdate);
   addLimitDRef(XPDR_ENG_LIMIT_CHT,    1, &SWB::engLimitCHTUpdate);
 
   addLimitDRef(XPDR_ENG_LIMIT_OILP,   1, &SWB::engLimitOilPUpdate);
   addLimitDRef(XPDR_ENG_LIMIT_OILT,   1, &SWB::engLimitOilTUpdate);
   addLimitDRef(XPDR_ENG_LIMIT_FUELP,  1, &SWB::engLimitFuelPUpdate);

   // Flaps
   addDirectDRef(XPDR_FLAP1_DEPLOY,    4, &SWB::flap1Update);
   addDirectDRef(XPDR_FLAP2_DEPLOY,    4, &SWB::flap2Update);


   foreach (int i, drmap.keys()) {
      DRefValue* val = drmap.value(i);
      QString vstr = val->str;
      qDebug() << "Dataref" << i << "(" << val->xpIndex << ") @" << val->freq << "hz:" << vstr;

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
      
      xplane->writeDatagram(data, len, settings->xplaneHost(), settings->xplanePortIn());
   }


   // Turn on UDP output
   // This works (the DSEL packet to xplane), at least in 10.40b7
   QList<XPDataIndex> indexes;
   // Be lazy and enable everything we can for output.  Better this than
   // explicitly selecting only the ones we want.
   for (int i = 0; i < NUM_DATA_INDEXES-1; i++) {
      indexes.append((XPDataIndex) i);
   }

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
   xplane->writeDatagram(dsel, len2, settings->xplaneHost(), settings->xplanePortIn());
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
      if (DEBUG_RECV_RREF)
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
         if (DEBUG_RECV_RREF && code >= 188 && code <= 199) // EPR/EGT only
            qDebug() << "   data received:" << header << dref->code << dref->data;

         notifyAll((int) code, value);
      }
      if (DEBUG_RECV_RREF)
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
      limit_fp    sigLimit    = val->signalLimit;
      if (sigDirect) {
         emit (this->*(sigDirect))(value);
      }
      if (sigNumbered) {
         emit (this->*(sigNumbered))(value, val->signalNum);
      }
      if (sigLimit) {
         emit (this->*(sigLimit))(value, val->limitType);
      }
   }
   else {
      if (DEBUG_RECV_RREF)
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
