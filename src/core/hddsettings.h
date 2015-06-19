/* 
 * File:   hddsettings.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 15, 2015, 6:40 PM
 */

#ifndef HDDSETTINGS_H
#define HDDSETTINGS_H

#include <QObject>
#include <QString>
#include <QHostAddress>

#include "mapconsts.h"

class QSettings;

class HDDSettings : public QObject {
   Q_OBJECT;

public:
   HDDSettings(QString _filename=0, QObject* _parent = 0);
   HDDSettings(const HDDSettings& orig) = delete;
   virtual ~HDDSettings();
   
   QString     configDir()    const { return m_configDir;      }
   QString     appRootDir()   const { return m_appRootDir;     }
   QString     userHomeDir()  const { return m_userHomeDir;    }
   QString     settingsFile() const { return m_settingsFile;   }
   
   int            xplanePort() const { return m_xplanePort; }
   QHostAddress   xplaneHost() const { return m_xplaneHost; }
   
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
   
   // settings values for the HDD app
   
   // "xplane" group
   int            m_xplanePort;
   QHostAddress   m_xplaneHost;
   
   // "map" group
   MapOrientation m_mapOrientation;
};

#endif	/* HDDSETTINGS_H */

