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
#include <QHostInfo>


CPDSettings::CPDSettings(QString _filename, DebugType _dt, QObject* _parent)
: QObject(_parent),
m_debug(_dt),
m_isMCSDataSwitch(false) {
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

//CPDSettings::CPDSettings(const CPDSettings& orig) {
//}

CPDSettings::~CPDSettings() {
}


SlaveSystem* CPDSettings::getSlaveByName(QString name) const {
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


QHostAddress& CPDSettings::getDestHost(QHostAddress& src) {
  foreach (int id, m_slaves.keys()) {
    SlaveSystem* sys = m_slaves.value(id);
    if (sys->m_xplaneHost == src) {
      return sys->m_cpdHost;
    }
  }
  QHostAddress host;
  return host;
}


void CPDSettings::loadSettingsFile(QString _filename) {
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
  // m_isMCSDataSwitch = settings->value("is_mcsdataswitch", "false").toBool();
  m_isMCSDisplay    = settings->value("is_mcsdisplay", "false").toBool();
  m_forwardToMCS    = settings->value("is_mcsdisplay", "false").toBool();
  QString mcsdh     = settings->value("mcs_display_host").toString();
  m_mcsDisplayHost  = checkHost(mcsdh);
  QString mcsdsh    = settings->value("mcs_data_switch_host").toString();
  m_mcsDataSwitchHost = checkHost(mcsdsh);
  settings->endGroup();  // "mcs"

  if (isMCS()) {
    // Load X-Plane 10 slave settings
    m_numSlaves = settings->beginReadArray("xplane-slaves");
    qDebug() << "Loading MCS settings for" << m_numSlaves << "slaves...";
    for (int i = 0; i < m_numSlaves; ++i) {
      settings->setArrayIndex(i);
      SlaveSystem* slave = new SlaveSystem();
      slave->m_slaveID          = i;
      slave->m_slaveName        = settings->value("slave_name").toString();
      slave->m_allowMCSOverride = settings->value("allow_mcs_override", "true").toBool();
      slave->m_xplanePortOut    = settings->value("xplane_port_out").toInt();
      slave->m_xplanePortIn     = settings->value("xplane_port_in").toInt();
      slave->m_xplanePluginPort = settings->value("xplane_plugin_port").toInt();
      QString xplaneH           = settings->value("xplane_host").toString();
      slave->m_xplaneHost       = checkHost(xplaneH);

      QString cpdH              = settings->value("cpd_host").toString();
      slave->m_cpdHost          = checkHost(cpdH);
      slave->m_cpdPortOut       = settings->value("cpd_port_out", slave->m_xplanePortIn).toInt();
      slave->m_cpdPortIn        = settings->value("cpd_port_in", slave->m_xplanePortOut).toInt();
      
      slave->m_mcsPortOut       = settings->value("mcs_port_out").toInt();
      slave->m_mcsPortIn        = settings->value("mcs_port_in").toInt();
      
      m_slaves.insert(i, slave);
    }
    settings->endArray();  // "xplane-slaves"
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
      m_xplaneHost = checkHost(host);
    }
    settings->endGroup();  // "xplane"
    // Set default values for MCS data
    m_numSlaves = 0;
  }
}

QHostAddress CPDSettings::checkHost(QString h) {
  QHostAddress mcsDispHost(h);
  if (debugSettings()) {
    qDebug() << "Validating host" << h;
  }
  // If the host given was a hostname...
  if (mcsDispHost == QHostAddress("")) {
    // Do a reverse-lookup to get the IP
    QHostInfo info = QHostInfo::fromName(h);
    QList<QHostAddress> addrs = info.addresses();
    if (debugSettings() && addrs.size()) {
      qDebug() << "  Found hosts for reverse lookup of\n" << h << "\n  :" << addrs;
    }
    if (!addrs.size()) {
      qWarning() << "Warning: host not found, skipping...";
      return QHostAddress();
    }
    return addrs.at(0);
  } 
  // ...else, if the host was an IP...
  else {
    return mcsDispHost;
  }
}
