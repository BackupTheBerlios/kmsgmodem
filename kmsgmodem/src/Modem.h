/****************************************************************************
** Form interface generated from reading ui file './Modem.ui'
**
** Created: Sat Oct 16 15:06:09 2004
**      by: The User Interface Compiler ($Id: Modem.h,v 1.10 2004/10/16 13:25:03 wirr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef MODEM_H
#define MODEM_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QGroupBox;
class KLineEdit;
class QLabel;
class KComboBox;

class Modem : public QWidget
{
    Q_OBJECT

public:
    Modem( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~Modem();

    QGroupBox* groupBox2;
    KLineEdit* kcfg_Port;
    QLabel* textLabel1;
    KComboBox* kcfg_Baudrate;
    QLabel* textLabel2;
    QLabel* textLabel1_2;

protected:

protected slots:
    virtual void languageChange();

};

#endif // MODEM_H
