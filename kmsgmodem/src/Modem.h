/****************************************************************************
** Form interface generated from reading ui file './Modem.ui'
**
** Created: Sun Aug 22 19:09:47 2004
**      by: The User Interface Compiler ($Id: Modem.h,v 1.3 2004/08/23 20:48:08 wirr Exp $)
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
