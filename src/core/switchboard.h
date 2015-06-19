/*
   This class is like an old telegram switchboard, redirecting core data from
   X-Plane and other data sources (Google Maps?) to wherever it needs to go
   (the individual widgets for displaying).
 */

#include <QObject>


class QUdpSocket;
class HDDSettings;
class XPData;

class SwitchBoard : public QObject {
   Q_OBJECT

public:
   SwitchBoard(HDDSettings* _settings, QObject* _parent=0);
   SwitchBoard(const SwitchBoard& orig) = delete;
   ~SwitchBoard();
   
public slots:
   void readPendingData();
   
signals:
   void speedUpdate(float speed);
   
   void machNumUpdate(float mach);
   void vertVelUpdate(float vertvel);

   void pitchUpdate(float pitch);
   void rollUpdate(float roll);
   void headingTrueUpdate(float heading);
   void headingMagUpdate(float heading);

   void aoaAlphaUpdate(float aoaA);
   void aoaBetaUpdate(float aoaB);
   void hPathUpdate(float hpath);
   void vPathUpdate(float vpath);
   void sideSlipUpdate(float ss);

   void compassUpdate(float heading);
   void latLonUpdate(float lat, float lon, int aircraft); // in GPS coords/deg
   void altMSLUpdate(float alt); // in feet
   void altAGLUpdate(float alt); // in feet

private:
   HDDSettings* settings;
   QUdpSocket* xplane;
   
   void initSocket();
   void processDatagram(QByteArray& data);
   void notifyAll(XPData* data);
};
