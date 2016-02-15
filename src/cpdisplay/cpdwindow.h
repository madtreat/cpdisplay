
#ifndef CPDWINDOW_H
#define CPDWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QList>


class CPDSettings;
class CPDWidget;
class SwitchBoard;

class CPDWindow : public QMainWindow {
  Q_OBJECT;

public:
  CPDWindow(CPDSettings* _cpdSettings, QWidget* _parent=0);
  ~CPDWindow();

public slots:
  void displayMCSWidget();
  void displaySlaveCPD(int i);

private:
  CPDSettings*        settings;

  // For an MCS window:
  QTabWidget*         tabWidget;
  QList<CPDWidget*>   slaveCPDs;
  QList<SwitchBoard*> slaveSwitches;
  QWidget*            mcsWidget;  // MCS controls

  // For a standard CPD window:
  CPDWidget*          cpdWidget;
};

#endif
