/* 
 * File:   enginewidget.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 15, 2015, 2:37 PM
 */

#ifndef ENGINEWIDGET_
#define ENGINEWIDGET_

#include <QWidget>


class HDDSettings;


class EngineWidget : public QWidget {
   Q_OBJECT;

public:
   EngineWidget(HDDSettings* _hddSettings, QWidget* _parent = 0);
   EngineWidget(const EngineWidget& orig) = delete;
   virtual ~EngineWidget();

public slots:
   

private:
   HDDSettings* hddSettings;

   void setupEngineControls();
};

#endif	/* ENGINEWIDGET_ */

