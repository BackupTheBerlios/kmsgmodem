/****************************************************************************
** Form interface generated from reading ui file './kmsgmodemwidgetbase.ui'
**
** Created: Mon Aug 23 23:15:33 2004
**      by: The User Interface Compiler ($Id: kmsgmodemwidgetbase.h,v 1.4 2004/08/23 21:19:22 wirr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef KMSGMODEMWIDGETBASE_H
#define KMSGMODEMWIDGETBASE_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class KListView;
class QListViewItem;

class KMsgModemWidgetBase : public QWidget
{
    Q_OBJECT

public:
    KMsgModemWidgetBase( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~KMsgModemWidgetBase();

    KListView* MessageView;

public slots:
    virtual void MessageClicked(QListViewItem *item);

protected:
    QGridLayout* kmsgmodemwidgetbaseLayout;

protected slots:
    virtual void languageChange();

};

#endif // KMSGMODEMWIDGETBASE_H
