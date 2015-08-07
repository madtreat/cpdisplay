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

#include "core/mapconsts.h"

class QSettings;

class CPDSettings : public QObject {
   Q_OBJECT;

public:
   CPDSettings(QString _filename=0, QObject* _parent = 0);
   CPDSettings(const CPDSettings& orig) = delete;
   virtual ~CPDSettings();
   
   QString     configDir()       const { return m_configDir;      }
   QString     appRootDir()      const { return m_appRootDir;     }
   QString     userHomeDir()     const { return m_userHomeDir;    }
   QString     settingsFile()    const { return m_settingsFile;   }
   
   QString     layoutProfile()   const { return m_layoutProfile;  }
   
   int            xplanePort()   const { return m_xplanePort;     }
   QHostAddress   xplaneHost()   const { return m_xplaneHost;     }
   
   MapOrientation mapOrientation() const { return m_mapOrientation; }
   void        setMapOrientation(MapOrientation mo) { m_mapOrientation = mo; }

public slots:
   void loadSettingsFile(QString _filename);

private:
   QSettings*  settings;
   QString     m_configDir;
   QString     m_appRootDir;
   QString     m_userHomeDir;
   QString     m_settingsFile;
   
   // Settings values for the Cockpit Display app
   QString     m_layoutProfile; // LayoutProfile config file
   
   // "xplane" group
   int            m_xplanePort;
   QHostAddress   m_xplaneHost;
   
   // "map" group
   MapOrientation m_mapOrientation;
};

#endif	/* CPDSETTINGS_H */

