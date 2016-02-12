
#ifndef GEARWIDGET_H_
#define GEARWIDGET_H_

#include <QFrame>

#include "gearconsts.h"


class CPDSettings;
class GearController;
class SingleGearWidget;


class GearWidget : public QFrame {
  Q_OBJECT;

public:
  GearWidget(CPDSettings* _settings, GearController* _gearC, int _numGears=3, QFrame* _parent=0);
  ~GearWidget();

signals:
  void masterChanged(bool up);

private:
  CPDSettings* settings;
  GearController* gearC;
  int numGears;

  SingleGearWidget* master;
};

#endif // GEARWIDGET_H_
