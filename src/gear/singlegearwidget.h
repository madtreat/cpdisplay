
#ifndef SINGLEGEARWIDGET_H_
#define SINGLEGEARWIDGET_H_

#include <QFrame>

class QLabel;
class QPushButton;

class CPDSettings;
class GearController;
class ToggleInput;

class SingleGearWidget : public QFrame {
  Q_OBJECT;

public:
  SingleGearWidget(CPDSettings* _settings, GearController* _gearC, int _gearNum, QFrame* _parent=0);
  ~SingleGearWidget();

public slots:
  void setStatus(double percent); // 0 = gear up, 1 = gear deployed
  void setSwitch(bool up); // up or down

signals:
  void switchChanged(bool up); // up or down

private:
  CPDSettings* settings;
  GearController* gearC;
  // if gearNum is -1, this is treated as a master switch and will control all
  // others at the same time.
  int gearNum;

  QLabel*        label;
  QLabel*        status;
  ToggleInput*   gearSwitch;
};

#endif // SINGLEGEARWIDGET_H_
