/****************************************************************************
** Form interface generated from reading ui file './ModemSettings.ui'
**
** Created: Mon Aug 23 23:43:04 2004
**      by: The User Interface Compiler ($Id: ModemSettings.h,v 1.2 2004/08/23 21:54:23 wirr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef SETTINGS_H
#define SETTINGS_H

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

class settings : public QDialog
{
    Q_OBJECT

public:
    settings( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~settings();

    QFrame* line1;
    QLabel* textLabel1;
    QGroupBox* groupBox5;
    KIntSpinBox* Rings;
    QLabel* textLabel1_2;
    QGroupBox* groupBox7;
    QCheckBox* Fax;
    QCheckBox* Voice;
    QGroupBox* groupBox4;
    QLabel* faxIdLabel;
    KLineEdit* faxId;
    QGroupBox* groupBox6;
    KLineEdit* Pass;
    QLabel* textLabel2;
    QCheckBox* Dialup;

public slots:
    virtual void settingsChanged();

protected:

protected slots:
    virtual void languageChange();

};

#endif // SETTINGS_H
