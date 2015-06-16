/*
   This class is like an old telegram switchboard, redirecting core data from
   X-Plane and other data sources (Google Maps?) to wherever it needs to go
   (the individual widgets for displaying).
 */

#include <QObject>


class QUdpSocket;
class HDDSettings;

class SwitchBoard : public QObject {
   Q_OBJECT

public:
   SwitchBoard(HDDSettings* _settings, QObject* _parent=0);
   SwitchBoard(const SwitchBoard& orig) = delete;
   ~SwitchBoard();
   
public slots:
   void readPendingData();

private:
   HDDSettings* settings;
   QUdpSocket* xplane;
   
   void initSocket();
   void processDatagram(QByteArray& data);
};
