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

public slots:
   void loadSettingsFile(QString _filename);

private:
   QSettings*  settings;
   QString     m_configDir;
   QString     m_appRootDir;
   QString     m_userHomeDir;
   QString     m_settingsFile;
   
   // settings values for the HDD app
   int            m_xplanePort;
   QHostAddress   m_xplaneHost;
};

#endif	/* HDDSETTINGS_H */

