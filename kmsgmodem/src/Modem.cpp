#include <kdialog.h>
#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file '/home/wirr/Dokumente/Programme/Projekte/KMsgModem/kmsgmodem/src/Modem.ui'
**
** Created: Thu Aug 26 18:18:01 2004
**      by: The User Interface Compiler ($Id: Modem.cpp,v 1.9 2004/08/26 16:22:34 wirr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "Modem.h"

#include <qvariant.h>
#include <qgroupbox.h>
#include <klineedit.h>
#include <qlabel.h>
#include <kcombobox.h>
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
    setMinimumSize( QSize( 320, 280 ) );

    groupBox2 = new QGroupBox( this, "groupBox2" );
    groupBox2->setGeometry( QRect( 10, 10, 300, 110 ) );

    kcfg_Port = new KLineEdit( groupBox2, "kcfg_Port" );
    kcfg_Port->setGeometry( QRect( 160, 70, 90, 22 ) );
    kcfg_Port->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, kcfg_Port->sizePolicy().hasHeightForWidth() ) );
    kcfg_Port->setMinimumSize( QSize( 40, 0 ) );

    textLabel1 = new QLabel( groupBox2, "textLabel1" );
    textLabel1->setGeometry( QRect( 50, 30, 100, 22 ) );

    kcfg_Baudrate = new KComboBox( FALSE, groupBox2, "kcfg_Baudrate" );
    kcfg_Baudrate->setGeometry( QRect( 160, 30, 92, 22 ) );

    textLabel2 = new QLabel( groupBox2, "textLabel2" );
    textLabel2->setGeometry( QRect( 50, 70, 110, 22 ) );

    textLabel1_2 = new QLabel( this, "textLabel1_2" );
    textLabel1_2->setGeometry( QRect( 10, 210, 300, 40 ) );
    languageChange();
    resize( QSize(320, 284).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // tab order
    setTabOrder( kcfg_Baudrate, kcfg_Port );
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
    groupBox2->setTitle( tr2i18n( "Interface" ) );
    textLabel1->setText( tr2i18n( "Baudrate:" ) );
    kcfg_Baudrate->clear();
    kcfg_Baudrate->insertItem( tr2i18n( "230400" ) );
    kcfg_Baudrate->insertItem( tr2i18n( "115200" ) );
    kcfg_Baudrate->insertItem( tr2i18n( "57600" ) );
    kcfg_Baudrate->insertItem( tr2i18n( "38400" ) );
    kcfg_Baudrate->insertItem( tr2i18n( "19200" ) );
    kcfg_Baudrate->insertItem( tr2i18n( "9600" ) );
    kcfg_Baudrate->insertItem( tr2i18n( "4800" ) );
    kcfg_Baudrate->insertItem( tr2i18n( "2400" ) );
    textLabel2->setText( tr2i18n( "Interface:" ) );
    textLabel1_2->setText( tr2i18n( "<p align=\"center\">These settings take effect after\n"
"a restart of KMsgModem.</p>" ) );
}

#include "Modem.moc"
