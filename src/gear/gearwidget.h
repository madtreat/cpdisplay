
#ifndef GEARWIDGET_H_
#define GEARWIDGET_H_

#include <QFrame>

#include "gearconsts.h"


class CPDSettings;
class GearController;


class GearWidget : public QFrame {
   Q_OBJECT;

public:
   GearWidget(CPDSettings* _settings, GearController* _gearC, QFrame* _parent=0);
   ~GearWidget();

private:
   CPDSettings* settings;
   GearController* gearC;

   GearWidgets gears;
};

#endif // GEARWIDGET_H_
