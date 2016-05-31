#ifndef MCSDATASWITCH_H
#define MCSDATASWITCH_H

#include <QObject>
#include <QMap>
#include <QHostAddress>

#include "mcsconsts.h"

class CPDSettings;
class Switchboard;


class MCSDataSwitch : public QObject {
  Q_OBJECT;

public:
  explicit MCSDataSwitch(CPDSettings* _settings, QObject* _parent=0);
  explicit MCSDataSwitch(const MCSDataSwitch& orig) = delete;
  ~MCSDataSwitch();

public slots:

private:
  CPDSettings* mcsSettings;
  SimClientMap slaves;  // A map of each of the slave's quarters
};

#endif  // MCSDATASWITCH_H
