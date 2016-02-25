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

class QSettings;


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

class CPDSettings : public QObject {
  Q_OBJECT;

public:
  CPDSettings(QString _filename, QObject* _parent = 0);
  CPDSettings(const CPDSettings& orig) = delete;
  virtual ~CPDSettings();

  QString     configDir()       const { return m_configDir;      }
  QString     appRootDir()      const { return m_appRootDir;     }
  QString     userHomeDir()     const { return m_userHomeDir;    }
  QString     settingsFile()    const { return m_settingsFile;   }

  QString     layoutProfile()   const { return m_layoutProfile;  }
  QString     styleFile()       const { return m_style;          }
  QString     mapSettingsFile() const { return m_mapSettings;    }

  bool        isMCS()           const { return m_isMCS;          }
  int         numSlaves()       const { return m_numSlaves;      }

  QMap<int, SlaveSystem*> slaves()  const { return m_slaves;           }
  SlaveSystem*   getSlave(int id)   const { return m_slaves.value(id); }
  SlaveSystem*   getSlaveByName(QString name) const;

  int            xplanePortOut()    const { return m_xplanePortOut;    }
  int            xplanePortIn()     const { return m_xplanePortIn;     }
  int            xplanePluginPort() const { return m_xplanePluginPort; }
  QHostAddress   xplaneHost()       const { return m_xplaneHost;       }

public slots:
  void loadSettingsFile(QString _filename);

private:
  QSettings*  settings;
  QString     m_configDir;
  QString     m_appRootDir;
  QString     m_userHomeDir;
  QString     m_settingsFile;

  // Settings values for the Cockpit Display app
  QString     m_layoutProfile;  // LayoutProfile config file
  QString     m_style;          // QSS style file
  QString     m_mapSettings;    // map settings file

  // "mcs" group
  bool        m_isMCS;          // Is this a MCS (Master Control System)?
  int         m_numSlaves;      // Number of slaves under this MCS

  // "xplane-slaves" group for MCS
  QMap<int, SlaveSystem*> m_slaves;  // Map of <ID, Slave info>

  // "xplane" group
  int            m_xplanePortOut;
  int            m_xplanePortIn;
  int            m_xplanePluginPort;
  QHostAddress   m_xplaneHost;
};

#endif	// CPDSETTINGS_H

