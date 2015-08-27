/*
 * File:   flapswidget.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on August 26, 2015
 */

#ifndef FLAPSWIDGET_H_
#define FLAPSWIDGET_H_

#include <QFrame>

class QSlider;

class CPDSettings;
class GearController;


class FlapsWidget : public QFrame {
   Q_OBJECT;

public:
   FlapsWidget(CPDSettings* _settings, GearController* _gearC, QFrame* _parent=0);
   ~FlapsWidget();

public slots:
//   void setFlaps();

signals:
//   void flapUpdate(float percent);

private:
   CPDSettings* settings;
   GearController* gearC;

   QSlider* flap1;
   QSlider* flap2;

   QSlider* createSlider();
};


#endif /* FLAPSWIDGET_H_ */
