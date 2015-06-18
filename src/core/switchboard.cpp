#include "switchboard.h"

#include <netinet/in.h>
#include <bitset>
#include <iostream>
#include <QUdpSocket>
#include <QByteArray>

#include "hddsettings.h"
#include "xplanedata.h"

SwitchBoard::SwitchBoard(HDDSettings* _settings, QObject* _parent)
: QObject(_parent)
{
   settings = _settings;
   initSocket();
}


SwitchBoard::~SwitchBoard()
{
}


void SwitchBoard::initSocket()
{
   xplane = new QUdpSocket(this);
   xplane->bind(QHostAddress::LocalHost, settings->xplanePort()); // TODO: make this a config option
   
   connect(xplane, SIGNAL(readyRead()), this, SLOT(readPendingData()));
}

void SwitchBoard::readPendingData()
{
   while (xplane->hasPendingDatagrams()) {
      QByteArray datagram;
      datagram.resize(xplane->pendingDatagramSize());
      QHostAddress sender;
      quint16 senderPort;
      
      xplane->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
      
      processDatagram(datagram);
   }
}

void SwitchBoard::processDatagram(QByteArray& data)
{
   // Remove the first 5 bytes to get the raw values
   QByteArray header = data.mid(0, 5);
   QByteArray values = data.remove(0, 5);
   
   // Each raw value is 36 bytes: 4 bytes=index from X-Plane, 32 bytes of data
   int numValues = values.size()/36;
   qDebug() << "Processing datagram of size:" << data.size() << numValues;
//   qDebug() << "   Header:" << header.data();
   
   // Separate each value
   for (int i = 0; i < numValues; i++) {
      // Get the 36 bytes starting at i*36
      QByteArray valueBytes = values.mid(i*36, 36);
      XPData* data = new XPData();
      data->parseRawData(valueBytes);
      // do something with data
      qDebug() << "   Index:" << data->index;
   }
}