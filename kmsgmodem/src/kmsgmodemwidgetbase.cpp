#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './kmsgmodemwidgetbase.ui'
**
** Created: Mon Aug 23 23:43:36 2004
**      by: The User Interface Compiler ($Id: kmsgmodemwidgetbase.cpp,v 1.6 2004/08/23 21:54:23 wirr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "kmsgmodemwidgetbase.h"

#include <qvariant.h>
#include <qheader.h>
#include <klistview.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a KMsgModemWidgetBase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
KMsgModemWidgetBase::KMsgModemWidgetBase( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "kmsgmodemwidgetbase" );
    setMinimumSize( QSize( 100, 50 ) );
    kmsgmodemwidgetbaseLayout = new QGridLayout( this, 1, 1, 11, 6, "kmsgmodemwidgetbaseLayout"); 

    MessageView = new KListView( this, "MessageView" );
    MessageView->addColumn( tr2i18n( "#" ) );
    MessageView->addColumn( tr2i18n( "Type" ) );
    MessageView->addColumn( tr2i18n( "Date" ) );
    MessageView->addColumn( tr2i18n( "Size" ) );
    MessageView->addColumn( tr2i18n( "Sender" ) );
    MessageView->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, MessageView->sizePolicy().hasHeightForWidth() ) );
    MessageView->setMinimumSize( QSize( 500, 240 ) );
    MessageView->setCursor( QCursor( 13 ) );
    MessageView->setMidLineWidth( 0 );
    MessageView->setResizePolicy( KListView::Default );
    MessageView->setProperty( "selectionMode", "Single" );
    MessageView->setAllColumnsShowFocus( TRUE );
    MessageView->setRootIsDecorated( FALSE );
    MessageView->setFullWidth( TRUE );

    kmsgmodemwidgetbaseLayout->addWidget( MessageView, 0, 0 );
    languageChange();
    resize( QSize(521, 272).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( MessageView, SIGNAL( clicked(QListViewItem*) ), this, SLOT( MessageClicked(QListViewItem*) ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
KMsgModemWidgetBase::~KMsgModemWidgetBase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void KMsgModemWidgetBase::languageChange()
{
    setCaption( tr2i18n( "kmsgmodemwidgetbase" ) );
    MessageView->header()->setLabel( 0, tr2i18n( "#" ) );
    MessageView->header()->setLabel( 1, tr2i18n( "Type" ) );
    MessageView->header()->setLabel( 2, tr2i18n( "Date" ) );
    MessageView->header()->setLabel( 3, tr2i18n( "Size" ) );
    MessageView->header()->setLabel( 4, tr2i18n( "Sender" ) );
}

void KMsgModemWidgetBase::MessageClicked(QListViewItem*)
{
    qWarning( "KMsgModemWidgetBase::MessageClicked(QListViewItem*): Not implemented yet" );
}

#include "kmsgmodemwidgetbase.moc"
