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


void CPDSettings::loadSettingsFile(QString _filename)
{
   // If the settings pointer already exists, delete it to start fresh on a new
   // file, since this can potentially be called multiple times in an application.
   if (settings) {
      qDebug() << "Settings object already exists, re-creating it" << settings;
      delete settings;
   }
   settings = new QSettings(_filename, QSettings::IniFormat);
   
   // Load general settings
   m_layoutProfile = m_configDir + "/" + settings->value("profile").toString();
   m_style         = m_configDir + "/" + settings->value("style").toString();
   m_mapSettings   = m_configDir + "/" + settings->value("map_settings").toString();

   // Load X-Plane 10 settings
   settings->beginGroup("xplane");
   m_xplanePortOut = settings->value("xplane_port_out").toInt();
   m_xplanePortIn  = settings->value("xplane_port_in").toInt();
   QString host    = settings->value("xplane_host").toString();
   if (host == "localhost") {
      m_xplaneHost = QHostAddress::LocalHost;
   }
   else {
      m_xplaneHost = QHostAddress(host);
   }
   settings->endGroup(); // "xplane"
}
