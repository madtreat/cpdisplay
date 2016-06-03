#ifndef CORECONSTS_H
#define CORECONSTS_H

#include <QMap>
#include <QString>
#include <QHostAddress>

#define DEBUG_DREF_ID 0
#define DEBUG_SEND 1
#define DEBUG_RECV_UDP 0
#define DEBUG_RECV_RREF 0
#define DEBUG_RECV (DEBUG_RECV_UDP | DEBUG_RECV_RREF)
#define DEBUG_RECV_DATASWITCH 0


enum ClientType {
  CLIENT_UNKNOWN = 0,
  CLIENT_CPD = 1 << 0,
  CLIENT_MCS = 1 << 1,
  CLIENT_XPLANE = 1 << 2
};

inline QString clientTypeStr(ClientType ct) {
  QString label = "Unknown";
  if (ct & CLIENT_CPD) {
    label = "CPD";
  }
  else if (ct & CLIENT_MCS) {
    label = "MCS";
  }
  else if (ct & CLIENT_XPLANE) {
    label = "XPlane";
  }
  return label;
}

enum MCSType {
  NOT_MCS = 0,
  MCSDisplay = 1 << 0,
  CPDataSwitch = 1 << 1
};

struct SlaveSystem {
  int           m_slaveID;
  QString       m_slaveName;
  bool          m_allowMCSOverride;

  QHostAddress  m_xplaneHost;
  int           m_xplanePortOut;
  int           m_xplanePortIn;
  int           m_xplanePluginPort;

  QHostAddress  m_cpdHost;
  int           m_cpdPortOut;
  int           m_cpdPortIn;

  int           m_mcsPortOut;
  int           m_mcsPortIn;
};

typedef QMap<int, SlaveSystem*> SlaveMap;

#endif  // CORECONSTS_H
