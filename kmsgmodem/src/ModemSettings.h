/****************************************************************************
** Form interface generated from reading ui file './ModemSettings.ui'
**
** Created: Sat Oct 16 15:06:08 2004
**      by: The User Interface Compiler ($Id: ModemSettings.h,v 1.6 2004/10/16 13:25:03 wirr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef MYMODEMSETTINGS_H
#define MYMODEMSETTINGS_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QFrame;
class QLabel;
class QGroupBox;
class KIntSpinBox;
class QCheckBox;
class KLineEdit;

class MyModemSettings : public QDialog
{
    Q_OBJECT

public:
    MyModemSettings( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~MyModemSettings();

    QFrame* line1;
    QLabel* textLabel1;
    QGroupBox* groupBox5;
    QLabel* textLabel1_2;
    KIntSpinBox* Rings;
    QGroupBox* groupBox7;
    QCheckBox* Fax;
    QCheckBox* Voice;
    QGroupBox* groupBox4;
    QLabel* faxIdLabel;
    KLineEdit* faxId;
    QGroupBox* groupBox6;
    QLabel* textLabel2;
    KLineEdit* Pass;
    QCheckBox* Dialup;

public slots:
    virtual void settingsChanged();
    virtual void Fax_stateChanged(int);

protected:

protected slots:
    virtual void languageChange();

};

#endif // MYMODEMSETTINGS_H
