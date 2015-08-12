
#ifndef GEARCONTROLLER_H_
#define GEARCONTROLLER_H_

#include <QObject>


class CPDSettings;


class GearController : public QObject {
   Q_OBJECT;

public:
   GearController(CPDSettings* _settings, QObject* _parent=0);
   ~GearController();

public slots:
   void setGear();

private:
   CPDSettings* settings;

};

typedef GearController GEARC;

#endif // GEARCONTROLLER_H_
