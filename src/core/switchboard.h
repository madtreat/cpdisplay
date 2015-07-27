/*
   This class is like an old telegram switchboard, redirecting core data from
   X-Plane and other data sources (Google Maps?) to wherever it needs to go
   (the individual widgets for displaying).
 */

#include <QObject>


class QUdpSocket;
class HDDSettings;
class XPOutputData;

class SwitchBoard : public QObject {
   Q_OBJECT

public:
   SwitchBoard(HDDSettings* _settings, QObject* _parent=0);
   SwitchBoard(const SwitchBoard& orig) = delete;
   ~SwitchBoard();
   
public slots:
   void readPendingData();
   
signals:
   void timeUpdate(float zulu, float local);
   void speedUpdate(float speed);
   
   void machNumUpdate(float mach);
   void vertVelUpdate(float vertvel);

   void pressureUpdate(float pressure);

   void angVelUpdate(float q, float p, float r);

   void pitchUpdate(float pitch);
   void rollUpdate(float roll);
   void headingTrueUpdate(float heading);
   void headingMagUpdate(float heading);

   void aoaSideSlipUpdate(float aoa, float sideslip);
   void hPathUpdate(float hpath);
   void vPathUpdate(float vpath);
   void slipSkidUpdate(float ss);

   void compassUpdate(float heading);
   void latLonUpdate(float lat, float lon); // in GPS coords/deg
   void altMSLUpdate(float alt); // in feet
   void altAGLUpdate(float alt); // in feet
   
   void acLatUpdate(float lat, int ac);
   void acLonUpdate(float lon, int ac);
   void acAltUpdate(float alt, int ac);
   
   void throttleCommandUpdate(float throttle, int engNum);
   void throttleActualUpdate(float throttle, int engNum);
   
   void engPowerUpdate(float power, int engNum);
   void engThrustUpdate(float thrust, int engNum);
   void engTorqueUpdate(float torque, int engNum);
   void engRPMUpdate(float rpm, int engNum);
   
   void com1Update(float freq, float standby);
   void com2Update(float freq, float standby);
   void comTransmitUpdate(float freq);
   
   void nav1Update(float freq, float standby);
   void nav2Update(float freq, float standby);

private:
   HDDSettings* settings;
   QUdpSocket* xplaneOutput;
   QUdpSocket* xplaneInput;
   
   void initSockets();
   void processDatagram(QByteArray& data);
   void notifyAll(XPOutputData* data);
};
