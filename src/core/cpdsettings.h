/*
 * File:   cpdsettings.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 15, 2015, 6:40 PM
 */

#ifndef CPDSETTINGS_H
#define CPDSETTINGS_H

#include <QObject>
#include <QString>
#include <QHostAddress>

#include "coreconsts.h"


class QSettings;


class CPDSettings : public QObject {
  Q_OBJECT;

public:
  CPDSettings(QString _filename, DebugType _dt = DEBUG_NONE, bool _isMCSDS = false, QObject* _parent = 0);
  CPDSettings(const CPDSettings& orig) = delete;
  virtual ~CPDSettings();

  QString     configDir()       const { return m_configDir; }
  QString     appRootDir()      const { return m_appRootDir; }
  QString     userHomeDir()     const { return m_userHomeDir; }
  QString     settingsFile()    const { return m_settingsFile; }

  // Debug flags
  void        setDebugFlags(DebugType flags) { m_debug = flags; }
  bool        debugDREFID()     const { return m_debug & DEBUG_DREF_ID; }
  bool        debugSend()       const { return m_debug & DEBUG_SEND; }
  bool        debugRecvUDP()    const { return m_debug & DEBUG_RECV_UDP; }
  bool        debugRecvPacket() const { return m_debug & DEBUG_RECV_PACKET; }
  bool        debugRecvRREF()   const { return m_debug & DEBUG_RECV_RREF; }
  bool        debugRecvDS()     const { return m_debug & DEBUG_RECV_DATASWITCH; }
  bool        debugSettings()   const { return m_debug & DEBUG_SETTINGS; }
  bool        debugForward()    const { return m_debug & DEBUG_FORWARD; }

  QString     layoutProfile()   const { return m_layoutProfile; }
  QString     styleFile()       const { return m_style; }
  QString     mapSettingsFile() const { return m_mapSettings; }

  bool        isMCS()           const { return m_isMCSDataSwitch || m_isMCSDisplay; }
  bool        isMCSDataSwitch() const { return m_isMCSDataSwitch; }
  bool        isMCSDisplay()    const { return m_isMCSDisplay; }
  bool        forwardToMCS()    const { return m_forwardToMCS; }
  int         numSlaves()       const { return m_numSlaves; }

  QHostAddress  mcsDisplayHost()    const { return m_mcsDisplayHost; }
  QHostAddress  mcsDataSwitchHost() const { return m_mcsDataSwitchHost; }

  SlaveMap      slaves()            const { return m_slaves; }
  SlaveSystem*  getSlave(int id)    const { return m_slaves.value(id); }
  SlaveSystem*  getSlaveByName(QString name) const;

  int           xplanePortOut()     const { return m_xplanePortOut; }
  int           xplanePortIn()      const { return m_xplanePortIn; }
  int           xplanePluginPort()  const { return m_xplanePluginPort; }
  QHostAddress  xplaneHost()        const { return m_xplaneHost; }

  QHostAddress  getDestHost(const QHostAddress& src);
  // int            getDestPort(int port);

public slots:
  void loadSettingsFile(QString _filename);

private:
  QSettings*  settings;
  QString     m_configDir;
  QString     m_appRootDir;
  QString     m_userHomeDir;
  QString     m_settingsFile;

  DebugType    m_debug;

  // Settings values for the Cockpit Display app
  QString m_layoutProfile;  // LayoutProfile config file
  QString m_style;          // QSS style file
  QString m_mapSettings;    // map settings file

  // "mcs" group
  bool m_isMCSDataSwitch;   // Is this a MCSDataSwitch host?
  bool m_isMCSDisplay;      // Is this a MCSDisplay host?
  bool m_forwardToMCS;      // Forward packets between CPD and xplane to MCS?
  int  m_numSlaves;         // Number of slaves under this MCS
  QHostAddress m_mcsDisplayHost;
  QHostAddress m_mcsDataSwitchHost;

  // "xplane-slaves" group for MCS
  SlaveMap m_slaves;  // Map of <ID, Slave info>

  // "xplane" group
  int            m_xplanePortOut;
  int            m_xplanePortIn;
  int            m_xplanePluginPort;
  QHostAddress   m_xplaneHost;

  QHostAddress checkHost(QString h);
};

#endif	// CPDSETTINGS_H

