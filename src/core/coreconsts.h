#ifndef CORECONSTS_H
#define CORECONSTS_H

#include <QMap>
#include <QString>
#include <QHostAddress>


enum MCSType {
  NOT_MCS = 0,
  MCSDisplay = 1 << 0,
  CPDataSwitch = 1 << 1
};

struct SlaveSystem {
  int            m_slaveID;
  QString        m_slaveName;
  bool           m_allowMCSOverride;

  int            m_xplanePortOut;
  int            m_xplanePortIn;
  int            m_xplanePluginPort;
  QHostAddress   m_xplaneHost;

  QHostAddress   m_cpdHost;
};

typedef QMap<int, SlaveSystem*> SlaveMap;

#endif  // CORECONSTS_H
