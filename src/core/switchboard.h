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


class QTimer;
class QUdpSocket;

class CPDSettings;
class XPOutputData;


class SwitchBoard : public QObject {
   Q_OBJECT

   // Typdef generic signal function pointers for direct value updates...
   typedef void (SwitchBoard::*direct_fp)(float);
   // and value updates tied to a specific engine number, gear number, etc. ...
   typedef void (SwitchBoard::*numbered_fp)(float, int);
   // and value updates for engine limits ...
   typedef void (SwitchBoard::*limit_fp)(float, LimitType);

   // An enum for bit-masking the type(s) of signals to use for a particular
   // DRefValue instance.
   // enum SignalType {
   //    SIG_DIRECT     = 1 << 0,
   //    SIG_NUMBERED   = 1 << 1,
   //    SIG_LIMIT      = 1 << 2
   // };

   // Define the value struct for the Dataref Map drmap
   struct DRefValue {
      int      xpIndex; // The index as defined by the application
                        //  (simple version: xpIndex == XPDataIndex)
      QString  str;     // The string representation of the dataref
      int      freq;    // Frequency of response
      // SignalType sigType;// Signal type(s) to be emitted

      /*
       * The SwitchBoard signal to be emitted, and the reason this struct is
       * inside the SwitchBoard class (the functions had to be accessed from 
       * inside).  All of these signals will be emitted by default (assuming
       * they are not null), so the application can use either or any, 
       * depending on context and what needs to be done.
       * 
       * If the dataref does not need one of these function pointers, then that
       * value can be safely set to null and no signal will not be emitted.
       */
      direct_fp   signalDirect;
      numbered_fp signalNumbered;
      limit_fp    signalLimit;

      /*
       * If using a numbered_fp, set this to the item's number to be emitted.
       * If not using a numbered_fp, set this to -1, which is the default in the
       * constructor for simplicity.  Note that the default is never used in 
       * this particular application because there is a macro to handle the
       * repetitive creation of DRefValue objects, and it requires a number.
       */
      int signalNum;

      /*
       * If using a limit_fp, set this to the corresponding type to be emitted
       * as one of the signal's arguments.
       */
      LimitType limitType;

      // Direct-signal-only constructor
      DRefValue(int _index, QString _str, int _freq, direct_fp _fn_d);
      
      // Numbered-signal-only constructor
      DRefValue(int _index, QString _str, int _freq, numbered_fp _fn_n, int _sigNum);
      
      // Limit-signal-only constructor
      DRefValue(int _index, QString _str, int _freq, limit_fp _fn_l, LimitType _limitType);
      
      // All-signals constructor: can use NULL to select only desired signal types
      DRefValue(int _index, QString _str, int _freq, direct_fp _fn_d, numbered_fp _fn_n, limit_fp _fn_l, int _sigNum=-1,  LimitType _limitType=NO_LIMIT);
      
      // Copy constructor
      DRefValue(const DRefValue& rhs);
   };

public:
   SwitchBoard(CPDSettings* _settings, QObject* _parent=0);
   SwitchBoard(const SwitchBoard& orig) = delete;
   ~SwitchBoard();
   
public slots:
   void testConnection();
   void readPendingData();
   void sendDREF(QString drefStr, xpflt value);
   void requestDatarefsFromXPlane();
   
signals:
   void notConnected(); // not connected to XPlane

   // XPlane 10.40+ versions:
   void acTailNumUpdate(float tail);
   void acNumEnginesUpdate(float num);

   // Engine Limits
   void engLimitMPUpdate   (float inhg,   LimitType type);
   void engLimitFFUpdate   (float gph,    LimitType type); // gal/hr
   void engLimitN1Update   (float percent,LimitType type);
   void engLimitN2Update   (float percent,LimitType type);

   void engLimitEPRUpdate  (float ratio,  LimitType type);
   void engLimitEGTUpdate  (float degC,   LimitType type);
   void engLimitTRQUpdate  (float ft_lb,  LimitType type);
   void engLimitITTUpdate  (float degC,   LimitType type);
   void engLimitCHTUpdate  (float degC,   LimitType type);

   void engLimitOilPUpdate (float psi,    LimitType type);
   void engLimitOilTUpdate (float degC,   LimitType type);
   void engLimitFuelPUpdate(float psi,    LimitType type);

   // Radios
   void radioCom1FreqUpdate (float freq);
   void radioCom1StdbyUpdate(float freq);
   void radioCom2FreqUpdate (float freq);
   void radioCom2StdbyUpdate(float freq);
   void radioNav1FreqUpdate (float freq);
   void radioNav1StdbyUpdate(float freq);
   void radioNav2FreqUpdate (float freq);
   void radioNav2StdbyUpdate(float freq);

   // Fuel Quantity
   void fuelQuantityUpdate(float qty, int tankNum);

   // Landing Gear - range: [0.0, 1.0]
   void gearRetractableUpdate(float rectractable); // a bool value
   void gearDeployUpdate(float percent, int gearNum);

   // Flaps - range: [0.0, 1.0]
   void flapUpdate(float percent);
   void flapHandleUpdate(float percent);


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

   QTimer* timer; // connection-test timer
   bool didReceiveData; // did this object receive data within the last timer?

   QMap<int, DRefValue*> drmap;

   // Helper functions for adding new entries to the drmap.
   void addDirectDRef   (QString str, int freq, direct_fp   sig);
   void addNumberedDRef (QString str, int freq, numbered_fp sig, int sigNum);
   void addLimitDRefHelp(QString str, int freq, limit_fp    sig, LimitType lt);
   void addLimitDRef    (QString str, int freq, limit_fp    sig);

   void processDatagram(QByteArray& data);
   void notifyAll(int code, xpflt value);
   void notifyAll(XPOutputData* data);

   void setDataref(QString dataref, float value);

   inline int nextDRefID() {return ++drefID;}
};

typedef SwitchBoard SWB;

#endif
