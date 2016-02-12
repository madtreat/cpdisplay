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

enum FlapWidgetType {
  FLAP_HANDLE,
  FLAP_REAL
};


class SingleFlapWidget : public QFrame {
  Q_OBJECT;

public:
  SingleFlapWidget(CPDSettings* _settings, GearController* _gearC, FlapWidgetType _type, QFrame* _parent=0);
  ~SingleFlapWidget();

public slots:
  void setFlap(float percent);
  void updateLabel(int percent);

private:
  CPDSettings* settings;
  GearController* gearC;
  FlapWidgetType type;

  QSlider* slider;
  QLabel* valueLabel;
};


#endif /* SINGLEFLAPWIDGET_H_ */
