
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
   void setGear();
   void notifyXPlaneGearHandle(bool up);

signals:
   void updateXPlaneGearHandle(QString dref, int down);

private:
   CPDSettings* settings;
   int numGears;
};

typedef GearController GEARC;

#endif // GEARCONTROLLER_H_
