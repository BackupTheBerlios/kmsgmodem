#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './Modem.ui'
**
** Created: Wed Aug 11 21:19:32 2004
**      by: The User Interface Compiler ($Id: Modem.cpp,v 1.1 2004/08/12 16:42:50 wirr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "Modem.h"

#include <qvariant.h>
#include <qlabel.h>
#include <klineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a Modem as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
Modem::Modem( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "Modem" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setMinimumSize( QSize( 0, 0 ) );
    ModemLayout = new QGridLayout( this, 1, 1, 11, 6, "ModemLayout"); 

    textLabel1 = new QLabel( this, "textLabel1" );

    ModemLayout->addWidget( textLabel1, 0, 0 );

    textLabel2 = new QLabel( this, "textLabel2" );

    ModemLayout->addWidget( textLabel2, 1, 0 );
    spacer1 = new QSpacerItem( 60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    ModemLayout->addItem( spacer1, 1, 2 );

    kcfg_Port = new KLineEdit( this, "kcfg_Port" );
    kcfg_Port->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, kcfg_Port->sizePolicy().hasHeightForWidth() ) );
    kcfg_Port->setMinimumSize( QSize( 40, 0 ) );

    ModemLayout->addWidget( kcfg_Port, 1, 1 );

    kcfg_Baudrate = new KLineEdit( this, "kcfg_Baudrate" );
    kcfg_Baudrate->setMinimumSize( QSize( 40, 0 ) );

    ModemLayout->addWidget( kcfg_Baudrate, 0, 1 );
    languageChange();
    resize( QSize(238, 114).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
Modem::~Modem()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void Modem::languageChange()
{
    setCaption( tr2i18n( "Form1" ) );
    textLabel1->setText( tr2i18n( "Baudrate:" ) );
    textLabel2->setText( tr2i18n( "Device:" ) );
}

#include "Modem.moc"
