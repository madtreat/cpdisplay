/*
   This class is like an old telegram switchboard, redirecting core data from
   X-Plane and other data sources (Google Maps?) to wherever it needs to go
   (the individual widgets for displaying).
 */

#include <QObject>


class SwitchBoard : public QObject {
   Q_OBJECT

public:
   SwitchBoard(QObject* _parent=0);
   ~SwitchBoard();

private:

}
