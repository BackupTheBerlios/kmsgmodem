/****************************************************************************
** Form interface generated from reading ui file '/home/wirr/Dokumente/Programme/Projekte/KMsgModem/kmsgmodem/src/kmsgmodemwidgetbase.ui'
**
** Created: Thu Aug 26 15:42:06 2004
**      by: The User Interface Compiler ($Id: kmsgmodemwidgetbase.h,v 1.7 2004/08/26 14:01:35 wirr Exp $)
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
