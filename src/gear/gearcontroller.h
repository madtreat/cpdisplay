
#ifndef GEARCONTROLLER_H_
#define GEARCONTROLLER_H_

#include <QObject>


class CPDSettings;


class GearController : public QObject {
   Q_OBJECT;

public:
   GearController(CPDSettings* _settings, int _numGears=3, QObject* _parent=0);
   ~GearController();

public slots:
   void notifyXPlaneGearHandle(bool up);
   void notifyXPlaneFlap1(float value);
   void notifyXPlaneFlap2(float value);

signals:
   void updateGearRetractable(float retractable);
   void updateXPlaneGearHandle(QString dref, int down);
   void updateXPlaneFlaps(QString dref, float value);

private:
   CPDSettings* settings;
   int numGears;
};

typedef GearController GEARC;

#endif // GEARCONTROLLER_H_
