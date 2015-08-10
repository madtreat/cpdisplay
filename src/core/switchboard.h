/*
   This class is like an old telegram switchboard, redirecting core data from
   X-Plane and other data sources (Google Maps?) to wherever it needs to go
   (the individual widgets for displaying).
 */

#ifndef SWITCHBOARD_H
#define SWITCHBOARD_H

#include <QObject>
#include <QMap>

#include "xplanedata.h"
#include "xplanedref.h"


class QUdpSocket;
class CPDSettings;
class XPOutputData;

class SwitchBoard : public QObject {
   Q_OBJECT

   // Typdef a generic signal function pointer
   typedef void (SwitchBoard::*func_pointer)(float);
   // Define the value struct for the Dataref Map drmap
   struct DRefValue {
      int            xpIndex; // The index as defined by the application
                              //  (simple version: xpIndex == XPDataIndex)
      QString        str;     // The string representation of the dataref
      func_pointer   signal;  // The SwitchBoard signal to be emitted, and
                              //  the reason this struct is inside SwitchBoard
      int            freq;    // Frequency of response

      DRefValue(int _index, QString _str, func_pointer _fn, int _freq) {
         xpIndex = _index;
         str = _str;
         signal = _fn;
         freq = _freq;
      }
      DRefValue(const DRefValue& rhs) {
         xpIndex = rhs.xpIndex;
         str = rhs.str;
         signal = rhs.signal;
         freq = rhs.freq;
      }
   };

public:
   SwitchBoard(CPDSettings* _settings, QObject* _parent=0);
   SwitchBoard(const SwitchBoard& orig) = delete;
   ~SwitchBoard();
   
public slots:
   void readPendingData();
   
signals:
   void notConnected(); // not connected to XPlane

   // XPlane 10.40+ versions:
   void acTailNumUpdate(float tail);
   void acNumEnginesUpdate(float num);

   void radioCom1FreqUpdate(float freq);
   void radioCom1StdbyUpdate(float freq);
   void radioCom2FreqUpdate(float freq);
   void radioCom2StdbyUpdate(float freq);
   void radioNav1FreqUpdate(float freq);
   void radioNav1StdbyUpdate(float freq);
   void radioNav2FreqUpdate(float freq);
   void radioNav2StdbyUpdate(float freq);

   void fuelQuantity1Update(float qty);
   void fuelQuantity2Update(float qty);
   void fuelQuantity3Update(float qty);
   void fuelQuantity4Update(float qty);
   void fuelQuantity5Update(float qty);
   void fuelQuantity6Update(float qty);
   void fuelQuantity7Update(float qty);
   void fuelQuantity8Update(float qty);
   void fuelQuantity9Update(float qty);


   // XPlane < 10.40 / raw UDP output versions:
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
   void propRPMUpdate(float rpm, int engNum);
   void propPitchUpdate(float pitch, int engNum);
   void propwashUpdate(float pwash, int engNum);
   
   void n1Update(float n1, int engNum);
   void n2Update(float n2, int engNum);
   void mpUpdate(float mp, int engNum);
   void eprUpdate(float epr, int engNum);
   void ffUpdate(float ff, int engNum);
   void ittUpdate(float itt, int engNum);
   void egtUpdate(float egt, int engNum);
   void chtUpdate(float cht, int engNum);
   
   void engOilPressureUpdate(float pressure, int engNum);
   void engOilTempUpdate(float temp, int engNum);
   
   void com1Update(float freq, float standby);
   void com2Update(float freq, float standby);
   void comTransmitUpdate(float freq);
   
   void nav1Update(float freq, float standby);
   void nav2Update(float freq, float standby);

private:
   CPDSettings* settings;
   QUdpSocket* xplane;
   int drefID; // used for incrementing dataref request ID's 

   QMap<int, DRefValue*> drmap;
   
   void initSocket();
   void requestDatarefsFromXPlane();
   void processDatagram(QByteArray& data);
   void notifyAll(int code, xpflt value);
   void notifyAll(XPOutputData* data);

   void setDataref(QString dataref, float value);

   int nextDRefID() {return ++drefID;}
};

typedef SwitchBoard SWB;

#endif
