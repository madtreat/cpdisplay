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
   
   int            xplanePortOut()const { return m_xplanePortOut;  }
   int            xplanePortIn() const { return m_xplanePortIn;   }
   QHostAddress   xplaneHost()   const { return m_xplaneHost;     }

   bool        mapUseProxy()     const { return m_mapUseProxy;    }
   QString     mapProxyHost()    const { return m_mapProxyHost;   }
   int         mapProxyPort()    const { return m_mapProxyPort;   }

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
   
   // "xplane" group
   int            m_xplanePortOut;
   int            m_xplanePortIn;
   QHostAddress   m_xplaneHost;

   // "map-proxy" group
   bool           m_mapUseProxy;
   QString        m_mapProxyHost;
   int            m_mapProxyPort;
};

#endif	/* CPDSETTINGS_H */

