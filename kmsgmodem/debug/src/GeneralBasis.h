/****************************************************************************
** Form interface generated from reading ui file '/home/wirr/Dokumente/Programme/Projekte/kmsgmodem/src/GeneralBasis.ui'
**
** Created: Fri Aug 6 21:55:44 2004
**      by: The User Interface Compiler ($Id: GeneralBasis.h,v 1.1 2004/08/12 16:42:54 wirr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef MYDIALOG1_H
#define MYDIALOG1_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QListBox;
class QListBoxItem;
class QTabWidget;
class QWidget;
class QPushButton;

class MyDialog1 : public QDialog
{
    Q_OBJECT

public:
    MyDialog1( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~MyDialog1();

    QListBox* listBox;
    QTabWidget* tabWidget;
    QWidget* Widget2;
    QWidget* Widget3;
    QPushButton* buttonHelp;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;

protected:
    QGridLayout* MyDialog1Layout;
    QHBoxLayout* Layout1;
    QSpacerItem* Horizontal_Spacing2;

protected slots:
    virtual void languageChange();

};

#endif // MYDIALOG1_H
