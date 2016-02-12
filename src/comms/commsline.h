/* 
 * File:   commsline.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on July 23, 2015, 12:01 PM
 */

#ifndef COMMSLINE_H
#define COMMSLINE_H

#include <QWidget>
#include <QString>

#include "comms.h"

class QLabel;
class QPushButton;
class QDoubleSpinBox;

class CommsController;


class CommsLine : public QWidget {
  Q_OBJECT;

public:
  CommsLine(CommsController* _comC, CommType _commType, QWidget* _parent = 0);
  CommsLine(const CommsLine& orig) = delete;
  virtual ~CommsLine();

public slots:
  void setActiveValue(float freq);
  void setStandbyValue(float freq);
  void swapActive();
  void setFreqs(CommType ct, float active, float standby);

  // These are for the spin boxes' valueChanged() signal
  void activeChanged(double freq);
  void standbyChanged(double freq);

signals:

private:
  CommsController* comC;
  CommType type;

  // Active and Standby Frequencies
  float activeFreq;
  float standbyFreq;
  QLabel* typeLabel;
  QLabel* active;
  QLabel* standby;
  QDoubleSpinBox* activeSB;
  QDoubleSpinBox* standbySB;

  // Increase/Decrease buttons
  QPushButton* actUp;
  QPushButton* actDown;
  QPushButton* standbyUp;
  QPushButton* standbyDown;

  // Button to Swap Active and Standby
  QPushButton* swap;

  // If this line just updated a value, ignore the next update signal
  bool activeJustChanged;
  bool standbyJustChanged;
  bool justInitialized; // dont send the first valueChanged

  QDoubleSpinBox* createFreqSpinBox();
  QString formatFreq(float freq);
};

#endif // COMMSLINE_H

