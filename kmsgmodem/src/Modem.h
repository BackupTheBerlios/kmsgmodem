/****************************************************************************
** Form interface generated from reading ui file './Modem.ui'
**
** Created: Mon Aug 16 15:29:13 2004
**      by: The User Interface Compiler ($Id: Modem.h,v 1.2 2004/08/16 14:18:20 wirr Exp $)
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
class QLabel;
class KLineEdit;

class Modem : public QWidget
{
    Q_OBJECT

public:
    Modem( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~Modem();

    QLabel* textLabel1;
    QLabel* textLabel2;
    KLineEdit* kcfg_Port;
    KLineEdit* kcfg_Baudrate;

protected:
    QGridLayout* ModemLayout;
    QSpacerItem* spacer1;

protected slots:
    virtual void languageChange();

};

#endif // MODEM_H
