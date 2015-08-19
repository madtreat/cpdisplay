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

   // Typdef generic signal function pointers for direct value updates...
   typedef void (SwitchBoard::*direct_fp)(float);
   // and value updates tied to a specific engine number, gear number, etc.
   typedef void (SwitchBoard::*numbered_fp)(float, int);

   // Define the value struct for the Dataref Map drmap
   struct DRefValue {
      int      xpIndex; // The index as defined by the application
                        //  (simple version: xpIndex == XPDataIndex)
      QString  str;     // The string representation of the dataref
      int      freq;    // Frequency of response

      /*
       * The SwitchBoard signal to be emitted, and the reason this struct is
       * inside the SwitchBoard class (the functions had to be accessed from 
       * inside).  Both of these signals will be emitted by default (assuming
       * they are not null), so the application can use either or both, 
       * depending on context and what needs to be done.
       * 
       * If the dataref does not need a numbered_fp or a direct_fp, then that
       * value can be safely set to null and no signal will not be emitted.
       */
      direct_fp   signalDirect;
      numbered_fp signalNumbered;

      /*
       * If using a numbered_fp, set this to the item's number to be emitted.
       * If not using a numbered_fp, set this to -1, which is the default in the
       * constructor for simplicity.  Note that the default is never used in 
       * this particular application because there is a macro to handle the
       * repetitive creation of DRefValue objects, and it requires a number.
       */
      int signalNum;

      DRefValue(int _index, QString _str, direct_fp _fn_d, numbered_fp _fn_n, int _freq, int _sigNum=-1) {
         xpIndex = _index;
         str = _str;
         signalDirect = _fn_d;
         signalNumbered = _fn_n;
         freq = _freq;
         signalNum = _sigNum;
      }

      DRefValue(const DRefValue& rhs) {
         xpIndex = rhs.xpIndex;
         str = rhs.str;
         signalDirect = rhs.signalDirect;
         signalNumbered = rhs.signalNumbered;
         freq = rhs.freq;
         signalNum = rhs.signalNum;
      }
   };

public:
   SwitchBoard(CPDSettings* _settings, QObject* _parent=0);
   SwitchBoard(const SwitchBoard& orig) = delete;
   ~SwitchBoard();
   
public slots:
   void readPendingData();
   void sendDREF(QString drefStr, xpflt value);
   
signals:
   void notConnected(); // not connected to XPlane

   // XPlane 10.40+ versions:
   void acTailNumUpdate(float tail);
   void acNumEnginesUpdate(float num);

   void radioCom1FreqUpdate( float freq);
   void radioCom1StdbyUpdate(float freq);
   void radioCom2FreqUpdate( float freq);
   void radioCom2StdbyUpdate(float freq);
   void radioNav1FreqUpdate( float freq);
   void radioNav1StdbyUpdate(float freq);
   void radioNav2FreqUpdate( float freq);
   void radioNav2StdbyUpdate(float freq);

   // Fuel Quantity
   void fuelQuantityUpdate(float qty, int tankNum);
   /* // These are not used because for loops are better
   void fuelQuantity0Update(float qty);
   void fuelQuantity1Update(float qty);
   void fuelQuantity2Update(float qty);
   void fuelQuantity3Update(float qty);
   void fuelQuantity4Update(float qty);
   void fuelQuantity5Update(float qty);
   void fuelQuantity6Update(float qty);
   void fuelQuantity7Update(float qty);
   void fuelQuantity8Update(float qty);
   // */

   // Landing Gear
   // range: [0.0, 1.0]
   void gearDeployUpdate(float percent, int gearNum);
   /* // These are not used because for loops are better
   void gear0DeployUpdate(float percent);
   void gear1DeployUpdate(float percent);
   void gear2DeployUpdate(float percent);
   void gear3DeployUpdate(float percent);
   void gear4DeployUpdate(float percent);
   void gear5DeployUpdate(float percent);
   void gear6DeployUpdate(float percent);
   void gear7DeployUpdate(float percent);
   void gear8DeployUpdate(float percent);
   void gear9DeployUpdate(float percent);
   // */


   // XPlane < 10.40 / raw UDP output versions:
   void timeUpdate(float zulu, float local, float mission, float timer);
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

   inline int nextDRefID() {return ++drefID;}
};

typedef SwitchBoard SWB;

#endif
