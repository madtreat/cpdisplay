/* 
 * File:   commswidget.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 15, 2015, 2:37 PM
 */

#ifndef COMMSWIDGET_
#define COMMSWIDGET_

#include <QWidget>

class QLabel;

class HDDSettings;


class CommsWidget : public QWidget {
   Q_OBJECT;

public:
   CommsWidget(HDDSettings* _hddSettings, QWidget* _parent = 0);
   CommsWidget(const CommsWidget& orig) = delete;
   virtual ~CommsWidget();
   
public slots:
   

private:
   HDDSettings* hddSettings;
   
   // The textual labels that will be connected to XPlane
   QLabel* com1;
   QLabel* com2;
   QLabel* nav1;
   QLabel* nav2;
   QLabel* gmt;
   QLabel* local;

   void setupCommsControls();
};

#endif	/* COMMSWIDGET_ */

