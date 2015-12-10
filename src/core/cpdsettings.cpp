/* 
 * File:   cpdsettings.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 15, 2015, 6:39 PM
 */

#include "cpdsettings.h"

#include <QCoreApplication>
#include <QSettings>
#include <QFile>
#include <QDir>

CPDSettings::CPDSettings(QString _filename, QObject* _parent)
: QObject(_parent)
{
   settings = NULL;
   m_userHomeDir = QDir::home().absolutePath();
   
   // Get config file from [application root directory]/config
   m_appRootDir = QCoreApplication::applicationDirPath();
   QDir appRoot(m_appRootDir);
   
   appRoot.cdUp();
   m_configDir = appRoot.absolutePath() + "/config";
   
   if (_filename == "") {
      m_settingsFile = m_configDir + "/cpd-settings.ini";
   }
   else {
      m_settingsFile = _filename;
      m_configDir = QFileInfo(m_settingsFile).absolutePath();
   }
   
   if (!QFile::exists(m_settingsFile)) {
      qWarning() << "Warning: Settings file" << m_settingsFile << "does not exist.";
      m_configDir = m_userHomeDir + "/.cpdisplay";
      m_settingsFile = m_configDir + "/cpd-settings.ini";
      qWarning() << "   Trying" << m_settingsFile << "...";
      if (!QFile::exists(m_settingsFile)) {
         qWarning() << "   File not found\n";
         qWarning() << "ERROR: No valid config files found.";
         qWarning() << "NOTE: you can specify a config file with the '-c <settings.ini>' option.";
         qWarning() << "Exiting.";
         exit(1);
      }
   }
   
   loadSettingsFile(m_settingsFile);
   qDebug() << "Loaded settings file:" << m_settingsFile;
}

//CPDSettings::CPDSettings(const CPDSettings& orig)
//{
//}

CPDSettings::~CPDSettings()
{
}

SlaveSystem* CPDSettings::getSlaveByName(QString name) const
{
   // The number of slaves should never be so large that a linear search
   // would be prohibitively slow.
   for (int i = 0; i < m_numSlaves; ++i) {
      SlaveSystem* slave = m_slaves.value(i);
      if (slave->m_slaveName == name) {
         return slave;
      }
   }
   return NULL;
}


void CPDSettings::loadSettingsFile(QString _filename)
{
   // If the settings pointer already exists, delete it to start fresh on a new
   // file, since this can potentially be called multiple times in an application.
   if (settings) {
      qDebug() << "Settings object already exists, re-creating it" << settings;
      delete settings;
   }
   settings = new QSettings(_filename, QSettings::IniFormat);
   qDebug() << "Child Keys/Groups:" << settings->childKeys() << "/" << settings->childGroups();
   
   // Load general settings
   m_layoutProfile = m_configDir + "/" + settings->value("profile").toString();
   m_style         = m_configDir + "/" + settings->value("style").toString();
   m_mapSettings   = m_configDir + "/" + settings->value("map_settings").toString();

   // Default to false if it does not appear (do not want standard CPD's
   // to be cluttered, also can prevent them from knowing about MCS)
   settings->beginGroup("mcs");
   m_isMCS = settings->value("is_mcs", "false").toBool();
   settings->endGroup(); // "mcs"

   if (m_isMCS) {
      // Load X-Plane 10 slave settings
      m_numSlaves = settings->beginReadArray("xplane-slaves");
      qDebug() << "Loading MCS settings for" << m_numSlaves << "slaves...";
      for (int i = 0; i < m_numSlaves; ++i) {
         settings->setArrayIndex(i);
         SlaveSystem* slave = new SlaveSystem();
         slave->m_slaveID           = i;
         slave->m_slaveName         = settings->value("slave_name").toString();
         slave->m_allowMCSOverride  = settings->value("allow_mcs_override", "true").toBool();
         slave->m_xplanePortOut     = settings->value("xplane_port_out").toInt();
         slave->m_xplanePortIn      = settings->value("xplane_port_in").toInt();
         slave->m_xplanePluginPort  = settings->value("xplane_plugin_port").toInt();
         slave->m_xplaneHost        = settings->value("xplane_host").toString();
         slave->m_cpdHost           = settings->value("cpd_host").toString();
         m_slaves.insert(i, slave);
      }
      settings->endArray(); // "xplane-slaves"
      // Set default values for m_xplane* data
      m_xplanePortOut = 0;
      m_xplanePortIn = 0;
      m_xplaneHost = "mcs";
   }
   else {
      // Load X-Plane 10 settings
      settings->beginGroup("xplane");
      m_xplanePortOut      = settings->value("xplane_port_out").toInt();
      m_xplanePortIn       = settings->value("xplane_port_in").toInt();
      m_xplanePluginPort   = settings->value("xplane_plugin_port").toInt();
      QString host         = settings->value("xplane_host").toString();
      if (host == "localhost") {
         m_xplaneHost = QHostAddress::LocalHost;
      }
      else {
         m_xplaneHost = QHostAddress(host);
      }
      settings->endGroup(); // "xplane"
      // Set default values for MCS data
      m_numSlaves = 0;
   }
   

   // Load Map Proxy settings
   settings->beginGroup("map-proxy");
   m_mapUseProxy  = settings->value("use_proxy").toBool();
   m_mapProxyHost = settings->value("proxy_host").toString();
   m_mapProxyPort = settings->value("proxy_port").toInt();
   settings->endGroup(); // "map-proxy"
}
