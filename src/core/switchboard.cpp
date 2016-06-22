/*
 *  This class has a few things that have not yet been accounted for:
 *    - XPlane plugins are not set up completely
 *    - XPlane plugins are not forwarded properly
 *        sendToPlugin()
 *    - sendDREF() is not set up to forward properly
 *    - requestDatarefsFromXPlane() not set up to forward properly
 *        2x data writes
 *    - 
 *    - 
 *    - THERE IS CURRENTLY NO BACK-FORWARDING
 *      so the CPD and MCS SwitchBoard instances are not set up to forward data
 *      to XPlane, "officially".  They can be configured in settings to treat
 *      the MCSDataSwitch host as the XPlane host, which is probably the 
 *      simplest method.
*/


#include "switchboard.h"

#include <netinet/in.h>
#include <bitset>
#include <iostream>
#include <unistd.h>
#include <string.h>

#include <QUdpSocket>
#include <QTcpSocket>
#include <QByteArray>
#include <QList>
#include <QDebug>
#include <QTimer>

// #include "coreconsts.h"
#include "cpdsettings.h"


// Direct-signal-only constructor
SwitchBoard::DRefValue::DRefValue(
  int _index,
  QString _str,
  int _freq,
  direct_fp _fn_d
) {
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
SwitchBoard::DRefValue::DRefValue(
  int _index,
  QString _str,
  int _freq,
  numbered_fp _fn_n,
  int _sigNum
) {
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
SwitchBoard::DRefValue::DRefValue(
  int _index,
  QString _str,
  int _freq,
  limit_fp _fn_l,
  LimitType _limitType
) {
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
SwitchBoard::DRefValue::DRefValue(
  int _index,
  QString _str,
  int _freq,
  direct_fp _fn_d,
  numbered_fp _fn_n,
  limit_fp _fn_l,
  int _sigNum,
  LimitType _limitType
) {
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


/*
 * slaveID is the identity of this machine if the
 */
SwitchBoard::SwitchBoard(
  CPDSettings* _settings,
  int _slaveID,
  bool _forward,
  QObject* _parent
) : QObject(_parent),
settings(_settings),
slaveID(_slaveID),
forwardToCPD(_forward),
drefID(NUM_DATA_INDEXES),
didReceiveData(false) {
  xplane = new QUdpSocket(this);
  xplanePlugin = new QTcpSocket(this);
  // connect(xplanePlugin, &QTcpSocket::readyRead, this, &SWB::readPluginData);
  // connect(xplanePlugin, &QTcpSocket::error, this, &SWB::pluginSocketError);

  // If this is a MCS subsystem
  if (settings->isMCS()) {
    SlaveSystem* slave = settings->getSlave(slaveID);

    // Set up the xplane host if this is a MCSDataSwitch
    if (settings->isMCSDataSwitch()) {
      xplaneHost        = slave->m_xplaneHost;
      xplanePortOut     = slave->m_xplanePortOut;
      xplanePortIn      = slave->m_xplanePortIn;
      xplanePluginPort  = slave->m_xplanePluginPort;
    }
    // If this is not a MCSDataSwitch (is a CPD or MCSDisplay)
    // In this case, the MCSDataSwitch machine is transparently presented
    // as the xplane machine, which is how the system works.
    else {
      xplaneHost        = settings->mcsDataSwitchHost();
      xplanePortOut     = slave->m_mcsPortIn;
      xplanePortIn      = slave->m_mcsPortOut;
      xplanePluginPort  = slave->m_xplanePluginPort; // TODO: figure this one out
    }

    groupName = settings->getSlave(slaveID)->m_slaveName;
    qDebug() << "Multi-sim setup group" << groupName << ":";
    qDebug() << "        xplane host:" << xplaneHost.toString() << "(out:" << xplanePortOut << ", in:" << xplanePortIn << ")";

    // Get CPD destination host and port from the xplane host and port numbers
    cpdHost     = slave->m_cpdHost;//TODO: necessary? settings->getDestHost(xplaneHost);
    cpdPortOut  = slave->m_cpdPortOut;
    cpdPortIn   = slave->m_cpdPortIn;

    cpd = new QUdpSocket(this);
    connect(cpd, &QUdpSocket::readyRead, this, &SwitchBoard::readFromCPD);
    cpd->bind(QHostAddress::Any, cpdPortOut, QUdpSocket::ShareAddress);

    qDebug() << "     cpdisplay host:" << cpdHost.toString()    << "(out:" << cpdPortOut    << ", in:" << cpdPortIn    << ")";

    // Set up MCS forwarding TODO: finish this
    mcsHost     = settings->mcsDisplayHost();
    mcsPortOut  = slave->m_mcsPortOut;
    mcsPortIn   = slave->m_mcsPortIn;

    mcs = new QUdpSocket(this);
    connect(mcs, &QUdpSocket::readyRead, this, &SwitchBoard::readFromMCS);
    mcs->bind(QHostAddress::Any, mcsPortOut, QUdpSocket::ShareAddress);

    qDebug() << "  mcscpdisplay host:" << mcsHost.toString()    << "(out:" << mcsPortOut    << ", in:" << mcsPortIn    << ")";
    qDebug() << "-------------------------------------------------------------------------";
  }
  // If standard CPD config...
  else {
    xplaneHost        = settings->xplaneHost();
    xplanePortOut     = settings->xplanePortOut();
    xplanePortIn      = settings->xplanePortIn();
    xplanePluginPort  = settings->xplanePluginPort();
    qDebug() << "xplane primary host:" << xplaneHost;

    cpdPortIn   = 0;
    cpdPortOut  = 0;
    mcsPortIn   = 0;
    mcsPortOut  = 0;
    cpd = NULL;
    mcs = NULL;
  }

  /*
   * This first function only works if xplane is running on this machine
   */
  if (xplaneHost == QHostAddress::LocalHost) {
    qDebug() << "Warning: X-Plane is running on localhost, some connection issues may occur.";
    xplane->bind(xplaneHost, xplanePortOut, QUdpSocket::ShareAddress);
  }
  /*
   * This second function only works if xplane is running on another machine
   * This socket must be bound to the input port of xplane to receive the raw
   * UDP output.  The xplane output port will cause raw UDP packets to be
   * ignored.
   */
  else {
    xplane->bind(QHostAddress::Any, xplanePortOut, QUdpSocket::ShareAddress);
  }
  connect(xplane, &QUdpSocket::readyRead, this, &SwitchBoard::readFromXPlane);

  // Create the map of Datarefs.
  buildDRMap();

  // Initialize the xplanePlugin TCP socket
  xplanePlugin->connectToHost(xplaneHost, xplanePluginPort);

  requestDatarefsFromXPlane();
  
  if (!settings->isMCSDisplay() || forwardToCPD) {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SwitchBoard::testConnection);
    timer->start(2000);
  }
}


SwitchBoard::~SwitchBoard() {
}


void SwitchBoard::testConnection() {
  // If we did receive data in the last timer interval, reset the flag
  if (didReceiveData) {
    didReceiveData = false;
  }
  // If we did not receive anything, request it again
  else {
    qWarning() << "Warning: no data from" << groupName << "xplane @" << xplaneHost.toString() << "; re-requesting.";
    requestDatarefsFromXPlane();
  }
}

void SwitchBoard::readFromCPD() {
  while (cpd->hasPendingDatagrams()) {
    readFromClient(CLIENT_CPD);
  }
}

void SwitchBoard::readFromMCS() {
  while (mcs->hasPendingDatagrams()) {
    readFromClient(CLIENT_MCS);
  }
}

void SwitchBoard::readFromXPlane() {
  while (xplane->hasPendingDatagrams()) {
    readFromClient(CLIENT_XPLANE);
  }
}

void SwitchBoard::readFromClient(ClientType ct) {
  // Get the correct client
  QUdpSocket* client = NULL;
  QString debugMsg = "Received data from " + groupName + " ";
  if (ct & CLIENT_CPD) {
    client = cpd;
    debugMsg += "cpd    @ " + cpdHost.toString() + ":" + QString::number(cpdPortOut);
  }
  else if (ct & CLIENT_MCS) {
    client = mcs;
    debugMsg += "mcs    @ " + mcsHost.toString() + ":" + QString::number(mcsPortOut);
  }
  else if (ct & CLIENT_XPLANE) {
    client = xplane;
    debugMsg += "xplane @ " + xplaneHost.toString() + ":" + QString::number(xplanePortOut);
  }
  if (DEBUG_RECV) {
    qDebug() << debugMsg;
  }

  // Read the data
  QByteArray datagram;
  datagram.resize(client->pendingDatagramSize());
  QHostAddress sender;
  quint16 senderPort;

  client->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

  // Debug if necessary
  if (DEBUG_RECV_PACKETS) {
    QString str = "Packet size %1 from (%2) %3:%4";
    str = str.arg(datagram.size()).arg(clientTypeStr(ct)).arg(sender.toString()).arg(senderPort);
    qDebug() << str;
  }

  // Process or forward the data
  if (DEBUG_FORWARD) {
    qDebug() << "Checking data forwarding for" << groupName << "...";
  }
  if (ct & CLIENT_CPD || ct & CLIENT_MCS) {
    if (DEBUG_FORWARD) {
      qDebug() << "  Data from CPD or MCS found!";
    }
    forwardData(CLIENT_XPLANE, datagram);
  }
  else if (ct & CLIENT_XPLANE) {
    if (DEBUG_FORWARD) {
      qDebug() << "  Data from xplane found!";
    }
    didReceiveData = true;

    if (forwardToCPD) {
      if (DEBUG_FORWARD) {
        qDebug() << "    Forwarding to the CPD...";
      }
      forwardData(CLIENT_CPD, datagram);

      if (settings->forwardToMCS()) {
        if (DEBUG_FORWARD) {
          qDebug() << "    Forwarding to the MCS Display...";
        }
        forwardData(CLIENT_MCS, datagram);
      }
    }
    else {
      processDatagram(datagram);
    }
  }
  if (DEBUG_FORWARD) {
    qDebug() << "done checking data forwarding.";
    qDebug() << "-------------------------------------------------------------";
  }

  // Unset the socket
  client = NULL;
}

void SwitchBoard::readPluginData() {
}

void SwitchBoard::pluginSocketError(QAbstractSocket::SocketError error) {
  qWarning() << "ERROR:" << xplanePlugin->errorString();
}

void SwitchBoard::sendDREF(QString drefStr, xpflt value) {
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

  xplane->writeDatagram(data, len, xplaneHost, xplanePortIn);
}

void SwitchBoard::sendToPlugin(QString data) {
  if (DEBUG_SEND)
    qDebug() << "Sending data to plugin:" << data;
  int bytesWritten = xplanePlugin->write(data.toLocal8Bit());
  if (bytesWritten == -1) {
    qDebug() << "Error sending data:" << xplanePlugin->errorString();
  }
}

void SwitchBoard::pauseSimulator(bool pause) {
  // TODO: figure out how to remotely pause the sim
  // since XPDR_TIME_PAUSED is read only
  QString cmd = "CMND=" + QString(XPDR_OP_PAUSE_TOGGLE);
  sendDREF(cmd, (float) pause);
}

void SwitchBoard::unpauseSimulator(bool unpause) {
  QString cmd = "CMND=" + QString(XPDR_OP_PAUSE_TOGGLE);
  sendDREF(cmd, (float) unpause);
}

void SwitchBoard::loadAircraft(QString path) {
}

void SwitchBoard::sendGearHandle(bool down) {
  QString str(XPDR_GEAR_HANDLE_STATUS);
  sendDREF(str, (int) down);
}

void SwitchBoard::sendFlapHandle(float value) {
  QString str(XPDR_FLAP_HANDLE_DEPLOY);
  sendDREF(str, value/100);
}

void SwitchBoard::sendBreaksOn(bool active) {
  qDebug() << "Sending breaks on?" << active << "--> NOT IMPLEMENTED";
  // QString str(XPDR_)
}

//void SwitchBoard::notifyComms(float value) {
//}


void SwitchBoard::addDirectDRef(QString str, int freq, direct_fp sig) {
  int id = nextDRefID();
  DRefValue* val = new DRefValue(id, str, freq, sig);
  drmap.insert(id, val);
}

void SwitchBoard::addNumberedDRef(QString str, int freq, numbered_fp sig, int sigNum) {
  int id = nextDRefID();
  DRefValue* val = new DRefValue(id, str, freq, sig, sigNum);
  drmap.insert(id, val);
}

void SwitchBoard::addLimitDRefHelp(QString str, int freq, limit_fp sig, LimitType lt) {
  int id = nextDRefID();
  DRefValue* val = new DRefValue(id, str, freq, sig, lt);
  drmap.insert(id, val);
}

void SwitchBoard::addLimitDRef(QString str, int freq, limit_fp sig) {
  addLimitDRefHelp(QString(str).replace("__LT__", "green_lo"),  freq, sig, LIMIT_G_LO);
  addLimitDRefHelp(QString(str).replace("__LT__", "green_hi"),  freq, sig, LIMIT_G_HI);
  addLimitDRefHelp(QString(str).replace("__LT__", "yellow_lo"), freq, sig, LIMIT_Y_LO);
  addLimitDRefHelp(QString(str).replace("__LT__", "yellow_hi"), freq, sig, LIMIT_Y_HI);
  addLimitDRefHelp(QString(str).replace("__LT__", "red_lo"),    freq, sig, LIMIT_R_LO);
  addLimitDRefHelp(QString(str).replace("__LT__", "red_hi"),    freq, sig, LIMIT_R_HI);
}


/*
 * Creates the DataRef map (drmap) ONE TIME ONLY.
 *
 * NOTES:
 *  - The dataref's ID is determined programmatically so you do not have
 *    to keep track of ID's for every single dataref you request.
 */
void SwitchBoard::buildDRMap() {
  // Request datarefs from xplane (does not work in < 10.40b7: known bug:
  // http://forums.x-plane.org/index.php?showtopic=87772)

  // Aircraft type and info
  //addDirectDRef(XPDR_AC_TYPE,           1, &SWB::acTypeUpdate);
  addDirectDRef(XPDR_AC_TAIL_NUM_1,     1, &SWB::acTailNumUpdate);
  addDirectDRef(XPDR_AC_NUM_ENGINES,    1, &SWB::acNumEnginesUpdate);

  // Radios
  addDirectDRef(XPDR_RADIO_COM1_FREQ,   1, &SWB::radioCom1FreqUpdate);
  addDirectDRef(XPDR_RADIO_COM1_STDBY,  1, &SWB::radioCom1StdbyUpdate);
  addDirectDRef(XPDR_RADIO_COM2_FREQ,   1, &SWB::radioCom2FreqUpdate);
  addDirectDRef(XPDR_RADIO_COM2_STDBY,  1, &SWB::radioCom2StdbyUpdate);
  addDirectDRef(XPDR_RADIO_NAV1_FREQ,   1, &SWB::radioNav1FreqUpdate);
  addDirectDRef(XPDR_RADIO_NAV1_STDBY,  1, &SWB::radioNav1StdbyUpdate);
  addDirectDRef(XPDR_RADIO_NAV2_FREQ,   1, &SWB::radioNav2FreqUpdate);
  addDirectDRef(XPDR_RADIO_NAV2_STDBY,  1, &SWB::radioNav2StdbyUpdate);

  // Fuel
  for (int i = 0; i < MAX_NUM_FUEL_TANKS; i++) {
    QString vstr = XPDR_CP_FUEL_QTY_X;
    vstr.replace("__X__", QString::number(i));
    addNumberedDRef(vstr, 4, &SWB::fuelQuantityUpdate, i);
  }
  addDirectDRef(XPDR_FUEL_WEIGHT,     10, &SWB::fuelQuantityUpdate);
  addDirectDRef(XPDR_FUEL_WEIGHT_TOT, 1,  &SWB::fuelMaxQuantityUpdate);

  // Gear
  addDirectDRef(XPDR_GEAR_RETRACTABLE,  1, &SWB::gearRetractableUpdate);
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
  addDirectDRef(XPDR_FLAP_DEPLOY,        4, &SWB::flapUpdate);
  addDirectDRef(XPDR_FLAP_HANDLE_DEPLOY, 4, &SWB::flapHandleUpdate);

  // Multiplayer
  for (int i = 1; i <= MAX_NUM_PLANES; i++) {
    QString vstr = QString(XPDR_PLANE_X_HEADING).replace("__X__", QString::number(i));
    addNumberedDRef(vstr, 20, &SWB::acHdgUpdate, i);

    vstr = QString(XPDR_PLANE_X_SPEED_X).replace("__X__", QString::number(i));
    addNumberedDRef(vstr, 20, &SWB::acSpdXUpdate, i);

    vstr = QString(XPDR_PLANE_X_SPEED_Y).replace("__X__", QString::number(i));
    addNumberedDRef(vstr, 20, &SWB::acSpdYUpdate, i);

    vstr = QString(XPDR_PLANE_X_SPEED_Y).replace("__X__", QString::number(i));
    addNumberedDRef(vstr, 20, &SWB::acSpdZUpdate, i);
  }

  // Misc
  addDirectDRef(XPDR_TIME_PAUSED,  2, &SWB::simPausedUpdate);
  addDirectDRef(XPDR_VSCL_ALPHA_D, 5, &SWB::aoaDUpdate);
}

/*
 * Sends the RREF message to XPlane to request all necessary datarefs.
 */
void SwitchBoard::requestDatarefsFromXPlane() {
  // If this SwitchBoard instance is running on a MCS Display, there is no need
  // to request datarefs or UDP output, as that all gets forwarded from the 
  // MCS DataSwitch's SwitchBoard instances as it forwards to the CPD.
  if (settings->isMCSDisplay()) {
    return;
  }

  // This loop sends the DREF requests to xplane
  foreach (int i, drmap.keys()) {
    DRefValue* val = drmap.value(i);
    QString vstr = val->str;
    if (DEBUG_DREF_ID)
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

    xplane->writeDatagram(data, len, xplaneHost, xplanePortIn);
  }


  // Turn on xplane's "simple" UDP output
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
  xplane->writeDatagram(dsel, len2, xplaneHost, xplanePortIn);
}


void SwitchBoard::forwardData(
  ClientType destCT,
  QByteArray& data
) {
  // TODO: get the sender, find the matching destination and forward it
  QUdpSocket* client = NULL;
  QHostAddress  host;
  quint16       port = 0;
  if (destCT & CLIENT_CPD) {
    client = cpd;
    host = cpdHost;
    port = cpdPortIn;
  }
  else if (destCT & CLIENT_MCS) {
    client = mcs;
    host = mcsHost;
    port = mcsPortIn;
  }
  else if (destCT & CLIENT_XPLANE) {
    client = xplane;
    host = xplaneHost;
    port = xplanePortIn;
  }

  // Write the data
  if (client) {
    client->writeDatagram(data, host, port);
  }

  // Unset the socket
  client = NULL;
}


void SwitchBoard::processDatagram(QByteArray& data) {
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
        qDebug() << "   data received:" << header << code << data;

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
      XPOutputData outData;
      outData.parseRawData(valueBytes);

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
void SwitchBoard::notifyAll(int code, xpflt value) {
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
  val = NULL;
  delete val;
}


/*
 * XPlane < 10.40 / raw UDP output version
 *
 * Notify everyone of new data.  This parses the data's values and emits signals
 * that other objects can be connected to.
 */
void SwitchBoard::notifyAll(const XPOutputData& data) {
  #define VALUE(pos) data.values.at(pos).toFloat()
  switch (data.index) {
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
      // Flip the signs for the velocity vector update to deal with
      // X-Plane coordinate weirdness.
      emit aoaSideSlipUpdate(-VALUE(0), -VALUE(1));
      emit hPathUpdate(VALUE(2));
      emit vPathUpdate(VALUE(3));
      // Also flip the sign on the slip skid to match the X-Plane PFD.
      emit slipSkidUpdate(-VALUE(7));
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

void SwitchBoard::setDataref(QString dataref, float value) {
   // TODO: send xp_dref_in message to xplane
}
