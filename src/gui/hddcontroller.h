/* 
 * File:   hddcontroller.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 15, 2015, 6:40 PM
 */

#ifndef HDDCONTROLLER_H
#define HDDCONTROLLER_H

#include <QObject>

class HDDSettings;
class SwitchBoard;
class HDDWindow;

class HDDController : public QObject {
   Q_OBJECT;

public:
   HDDController(HDDSettings* _settings, QObject* _parent = 0);
   HDDController(const HDDController& orig) = delete;
   virtual ~HDDController();

public slots:

private:
   HDDSettings*   settings;
   SwitchBoard*   sb;
   HDDWindow*     window;
   
   void connectSignals();
};

#endif	/* HDDCONTROLLER_H */

