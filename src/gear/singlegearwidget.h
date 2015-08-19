
#ifndef SINGLEGEARWIDGET_H_
#define SINGLEGEARWIDGET_H_

#include <QFrame>

class QLabel;
class QPushButton;

class CPDSettings;
class GearController;


class SingleGearWidget : public QFrame {
   Q_OBJECT;

public:
   SingleGearWidget(CPDSettings* _settings, GearController* _gearC, int _gearNum, QFrame* _parent=0);
   ~SingleGearWidget();

public slots:
   void setStatus(double percent); // 0 = gear up, 1 = gear deployed

private:
   CPDSettings* settings;
   GearController* gearC;
   int gearNum;

   QLabel*        label;
   QPushButton*   deployButton;
   QLabel*        status;
};

#endif // SINGLEGEARWIDGET_H_
