#include "switchboard.h"

#include <QUdpSocket>

#include "hddsettings.h"

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
   int numValues = (data.size()-5*4)/(4*4);
   qDebug() << "Processing datagram of size:" << data.size() << numValues;
}