/****************************************************************************
** Form interface generated from reading ui file './GeneralWidget.ui'
**
** Created: Mon Aug 16 15:29:13 2004
**      by: The User Interface Compiler ($Id: GeneralWidget.h,v 1.2 2004/08/16 14:18:20 wirr Exp $)
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
class QCheckBox;

class General : public QWidget
{
    Q_OBJECT

public:
    General( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~General();

    QCheckBox* kcfg_CheckOnStart;
    QCheckBox* kcfg_SetStandAloneModeOnExit;

protected:
    QGridLayout* GeneralLayout;

protected slots:
    virtual void languageChange();

};

#endif // GENERAL_H
