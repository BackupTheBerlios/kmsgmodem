/****************************************************************************
** Form interface generated from reading ui file './GeneralWidget.ui'
**
** Created: Sat Oct 16 15:06:08 2004
**      by: The User Interface Compiler ($Id: GeneralWidget.h,v 1.10 2004/10/16 13:25:02 wirr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef GENERAL_H
#define GENERAL_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QGroupBox;
class QCheckBox;
class QButtonGroup;
class QRadioButton;

class General : public QWidget
{
    Q_OBJECT

public:
    General( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~General();

    QGroupBox* groupBox1;
    QCheckBox* kcfg_SetStandAloneModeOnExit;
    QCheckBox* kcfg_CheckOnStart;
    QButtonGroup* buttonGroup1;
    QRadioButton* kcfg_NormalQuality;
    QRadioButton* kcfg_GoodQuality;

protected:

protected slots:
    virtual void languageChange();

};

#endif // GENERAL_H
