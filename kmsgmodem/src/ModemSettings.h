/****************************************************************************
** Form interface generated from reading ui file '/home/wirr/Dokumente/Programme/Projekte/KMsgModem/kmsgmodem/src/ModemSettings.ui'
**
** Created: Thu Aug 26 15:42:14 2004
**      by: The User Interface Compiler ($Id: ModemSettings.h,v 1.3 2004/08/26 14:01:35 wirr Exp $)
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
