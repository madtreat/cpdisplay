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
class SingleFlapWidget;


class FlapsWidget : public QFrame {
   Q_OBJECT;

public:
   FlapsWidget(CPDSettings* _settings, GearController* _gearC, QFrame* _parent=0);
   ~FlapsWidget();

private:
   CPDSettings* settings;
   GearController* gearC;

   SingleFlapWidget* flap1;
   SingleFlapWidget* flap2;
};


#endif /* FLAPSWIDGET_H_ */
