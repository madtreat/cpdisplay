/*
 * File:   singleflapwidget.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on August 26, 2015
 */

#ifndef SINGLEFLAPWIDGET_H_
#define SINGLEFLAPWIDGET_H_

#include <QFrame>


class QLabel;
class QSlider;

class CPDSettings;
class GearController;


class SingleFlapWidget : public QFrame {
   Q_OBJECT;

public:
   SingleFlapWidget(CPDSettings* _settings, GearController* _gearC, QFrame* _parent=0);
   ~SingleFlapWidget();

public slots:
   void updateLabel(int percent);

signals:
   void flapUpdate(float percent);

private:
   CPDSettings* settings;
   GearController* gearC;

   QSlider* slider;
   QLabel* label;
};


#endif /* SINGLEFLAPWIDGET_H_ */
