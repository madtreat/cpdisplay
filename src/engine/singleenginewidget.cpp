/* 
 * File:   singleenginewidget.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on July 31, 2015, 12:54 PM
 */

#include "singleenginewidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>

#include "core/cpdsettings.h"

#include "engineconsts.h"
#include "enginecontroller.h"
#include "enginedial.h"
#include "throttlewidget.h"


SingleEngineWidget::SingleEngineWidget(CPDSettings* _cpdSettings, EngineController* _engC, int _engNum, QFrame* _parent)
: QFrame(_parent),
  cpdSettings(_cpdSettings),
  engC(_engC),
  engNum(_engNum)
{
   // Define units for textual display values
   propPitchUnits = QString("%1").arg(QChar(0260));
   propWashUnits  = QString("kt");
   n1Units        = QString("%");
   n2Units        = QString("%");
   mpUnits        = QString("inhg");
   eprUnits       = QString("prt");
   ffUnits        = QString("lb/h");
   ittUnits       = QString("%1").arg(QChar(0260));
   egtUnits       = QString("%1").arg(QChar(0260));
   chtUnits       = QString("%1").arg(QChar(0260));
   
   
   // Create the layouts
   QVBoxLayout* layout = new QVBoxLayout(this);
   layout->setContentsMargins(4, 4, 4, 4);
   
   upper = new QHBoxLayout();
   
   propForm = new QWidget();
   QFormLayout* propFormLayout = new QFormLayout(propForm);
   propFormLayout->setContentsMargins(5, 0, 0, 0);
   
   jetForm = new QWidget();
   QFormLayout* jetFormLayout = new QFormLayout(jetForm);
   jetFormLayout->setContentsMargins(5, 0, 0, 0);
   
   
   // Create the widgets
   QString engineLabelString = QString("ENG %1").arg(engNum+1);
   QLabel* engineLabel = new QLabel(engineLabelString);
   engineLabel->setAlignment(Qt::AlignHCenter);
   engineLabel->setObjectName("border-light");
   
   throttle    = new ThrottleWidget(engC, engNum, 100, 0);
   oilP        = new EngineDial(engC, engNum, DIAL_OIL_PRESSURE, 40, 120);
   oilT        = new EngineDial(engC, engNum, DIAL_OIL_TEMP, 20, 180);
   engRPM      = new EngineDial(engC, engNum, DIAL_ENG_RPM, 0, 5000);
   propRPM     = new EngineDial(engC, engNum, DIAL_PROP_RPM, 0, 5000);
   eprDial     = new EngineDial(engC, engNum, DIAL_EPR, 0, 20);
   egtDial     = new EngineDial(engC, engNum, DIAL_EGT, 0, 2000);
   
   propPitch   = new QLabel("0 " + propPitchUnits);
   propWash    = new QLabel("0 " + propWashUnits);
   n1          = new QLabel("0 " + n1Units);
   n2          = new QLabel("0 " + n2Units);
   mp          = new QLabel("0 " + mpUnits);
   epr         = new QLabel("0 " + eprUnits);
   fuelFlow    = new QLabel("0 " + ffUnits);
   itt         = new QLabel("0 " + ittUnits);
   egt         = new QLabel("0 " + egtUnits);
   cht         = new QLabel("0 " + chtUnits);
   
   // Connect the widgets to the data source
   typedef SingleEngineWidget SEW;
   typedef EngineController ENGC;
   connect(engC, &ENGC::engineTypeUpdate, this, &SEW::setVisibleWidgets);
   connect(engC, &ENGC::engRPMUpdate,     this, &SEW::setEngRPM);
   connect(engC, &ENGC::propRPMUpdate,    this, &SEW::setPropRPM);
   connect(engC, &ENGC::propPitchUpdate,  this, &SEW::setPropPitch);
   connect(engC, &ENGC::propwashUpdate,   this, &SEW::setPropwash);
   connect(engC, &ENGC::n1Update,         this, &SEW::setN1);
   connect(engC, &ENGC::n2Update,         this, &SEW::setN2);
   connect(engC, &ENGC::mpUpdate,         this, &SEW::setMP);
   connect(engC, &ENGC::eprUpdate,        this, &SEW::setEPR);
   connect(engC, &ENGC::ffUpdate,         this, &SEW::setFF);
   connect(engC, &ENGC::ittUpdate,        this, &SEW::setITT);
   connect(engC, &ENGC::egtUpdate,        this, &SEW::setEGT);
   connect(engC, &ENGC::chtUpdate,        this, &SEW::setCHT);
   
   
   // Add the necessary form items to their respective widgets
   propFormLayout->addRow(tr("pitch"), propPitch);
   propFormLayout->addRow(tr("pwash"), propWash);
   propFormLayout->addRow(tr("MP"),    mp);
   propFormLayout->addRow(tr("FF"),    fuelFlow);
   propFormLayout->addRow(tr("ITT"),   itt);
   propFormLayout->addRow(tr("EGT"),   egt);
   propFormLayout->addRow(tr("CHT"),   cht);
   
   jetFormLayout->addRow(tr("pwash"),  propWash);
   jetFormLayout->addRow(tr("N1"),     n1);
   jetFormLayout->addRow(tr("N2"),     n2);
   jetFormLayout->addRow(tr("EPR"),    epr);
   jetFormLayout->addRow(tr("FF"),     fuelFlow);
   jetFormLayout->addRow(tr("ITT"),    itt);
   jetFormLayout->addRow(tr("EGT"),    egt);
   jetFormLayout->addRow(tr("CHT"),    cht);
   
   // Set visible widgets for this engine type
   setVisibleWidgets(engC->getEngineType());
   
   // Add widgets to the single engine layout
   upper->addWidget(throttle);
   upper->addWidget(jetForm);
   upper->addWidget(propForm);
   upper->addStretch(1);
   
   layout->addWidget(engineLabel);
   layout->addLayout(upper);
   layout->addWidget(oilP);
   layout->addWidget(oilT);
   layout->addWidget(engRPM);
   layout->addWidget(propRPM);
   layout->addWidget(eprDial);
   layout->addWidget(egtDial);
   layout->addStretch(2);
   
   setMinimumWidth(132);
   setObjectName("border-light");
}

//SingleEngineWidget::SingleEngineWidget(const SingleEngineWidget& orig)
//{
//}

SingleEngineWidget::~SingleEngineWidget()
{
}

void SingleEngineWidget::setVisibleWidgets(AircraftEngineType type)
{
   engRPM->setVisible(type == AC_ENG_PROP || type == AC_ENG_TURBOJET);
   propRPM->setVisible(type == AC_ENG_PROP || type == AC_ENG_TURBOJET);
   eprDial->setVisible(type == AC_ENG_JET || type == AC_ENG_TURBOJET);
   egtDial->setVisible(type == AC_ENG_JET || type == AC_ENG_TURBOJET);
   
   propForm->setVisible(type == AC_ENG_PROP || type == AC_ENG_TURBOJET);
   jetForm->setVisible(type == AC_ENG_JET);
}

void SingleEngineWidget::setEngRPM(float value, int _engNum)
{
   if (_engNum == engNum) {
      engRPM->setValue(value, engNum);
   }
}

void SingleEngineWidget::setPropRPM(float value, int _engNum)
{
   if (_engNum == engNum) {
      propRPM->setValue(value, engNum);
   }
}

void SingleEngineWidget::setPropPitch(float value, int _engNum)
{
   if (_engNum == engNum) {
      QString str = QString("%1 " + propPitchUnits).arg(value, 4, 'f', 1, '0');
      propPitch->setText(str);
   }
}

void SingleEngineWidget::setPropwash(float value, int _engNum)
{
   if (_engNum == engNum) {
      QString str = QString("%1 " + propWashUnits).arg(value, 4, 'f', 0, '0');
      propWash->setText(str);
   }
}

void SingleEngineWidget::setN1(float value, int _engNum)
{
   if (_engNum == engNum) {
      QString str = QString("%1 " + n1Units).arg(value, 4, 'f', 1, '0');
      n1->setText(str);
   }
}

void SingleEngineWidget::setN2(float value, int _engNum)
{
   if (_engNum == engNum) {
      QString str = QString("%1 " + n2Units).arg(value, 4, 'f', 1, '0');
      n2->setText(str);
   }
}

void SingleEngineWidget::setMP(float value, int _engNum)
{
   if (_engNum == engNum) {
      QString str = QString("%1 " + mpUnits).arg(value, 4, 'f', 1, '0');
      mp->setText(str);
   }
}

void SingleEngineWidget::setEPR(float value, int _engNum)
{
   if (_engNum == engNum) {
      QString str = QString("%1 " + eprUnits).arg(value, 2, 'f', 2, '0');
      epr->setText(str);
   }
}

void SingleEngineWidget::setFF(float value, int _engNum)
{
   if (_engNum == engNum) {
      QString str = QString("%1 " + ffUnits).arg(value, 5, 'f', 0, '0');
      fuelFlow->setText(str);
   }
}

void SingleEngineWidget::setITT(float value, int _engNum)
{
   if (_engNum == engNum) {
      QString str = QString("%1 " + ittUnits).arg(value, 4, 'f', 1, '0');
      itt->setText(str);
   }
}

void SingleEngineWidget::setEGT(float value, int _engNum)
{
   if (_engNum == engNum) {
      QString str = QString("%1 " + egtUnits).arg(value, 4, 'f', 1, '0');
      egt->setText(str);
   }
}

void SingleEngineWidget::setCHT(float value, int _engNum)
{
   if (_engNum == engNum) {
      
      QString str = QString("%1 " + chtUnits).arg(value, 4, 'f', 1, '0');
      cht->setText(str);
   }
}

void SingleEngineWidget::setOilPressure(float value, int _engNum)
{
   if (_engNum == engNum) {
      oilP->setValue(value, engNum);
   }
}

void SingleEngineWidget::setOilTemp(float value, int _engNum)
{
   if (_engNum == engNum) {
      oilT->setValue(value, engNum);
   }
}
