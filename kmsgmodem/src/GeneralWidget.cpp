#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file '/home/wirr/Dokumente/Programme/Projekte/KMsgModem/kmsgmodem/src/GeneralWidget.ui'
**
** Created: Thu Aug 26 15:42:45 2004
**      by: The User Interface Compiler ($Id: GeneralWidget.cpp,v 1.7 2004/08/26 14:01:35 wirr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "GeneralWidget.h"

#include <qvariant.h>
#include <qgroupbox.h>
#include <qcheckbox.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a General as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
General::General( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "General" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setMinimumSize( QSize( 330, 280 ) );

    groupBox1 = new QGroupBox( this, "groupBox1" );
    groupBox1->setGeometry( QRect( 10, 10, 310, 130 ) );

    kcfg_SetStandAloneModeOnExit = new QCheckBox( groupBox1, "kcfg_SetStandAloneModeOnExit" );
    kcfg_SetStandAloneModeOnExit->setGeometry( QRect( 20, 80, 280, 20 ) );
    kcfg_SetStandAloneModeOnExit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)4, (QSizePolicy::SizeType)4, 0, 0, kcfg_SetStandAloneModeOnExit->sizePolicy().hasHeightForWidth() ) );

    kcfg_CheckOnStart = new QCheckBox( groupBox1, "kcfg_CheckOnStart" );
    kcfg_CheckOnStart->setGeometry( QRect( 20, 40, 280, 20 ) );
    kcfg_CheckOnStart->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)4, (QSizePolicy::SizeType)4, 0, 0, kcfg_CheckOnStart->sizePolicy().hasHeightForWidth() ) );

    buttonGroup1 = new QButtonGroup( this, "buttonGroup1" );
    buttonGroup1->setGeometry( QRect( 10, 160, 310, 110 ) );

    kcfg_NormalQuality = new QRadioButton( buttonGroup1, "kcfg_NormalQuality" );
    kcfg_NormalQuality->setGeometry( QRect( 20, 30, 280, 20 ) );
    kcfg_NormalQuality->setChecked( TRUE );

    kcfg_GoodQuality = new QRadioButton( buttonGroup1, "kcfg_GoodQuality" );
    kcfg_GoodQuality->setGeometry( QRect( 20, 70, 280, 20 ) );
    languageChange();
    resize( QSize(330, 284).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // tab order
    setTabOrder( kcfg_CheckOnStart, kcfg_SetStandAloneModeOnExit );
    setTabOrder( kcfg_SetStandAloneModeOnExit, kcfg_NormalQuality );
}

/*
 *  Destroys the object and frees any allocated resources
 */
General::~General()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void General::languageChange()
{
    setCaption( tr2i18n( "Form1" ) );
    groupBox1->setTitle( tr2i18n( "Start && Exit" ) );
    kcfg_SetStandAloneModeOnExit->setText( tr2i18n( "Set standalone mode on exit" ) );
    kcfg_CheckOnStart->setText( tr2i18n( "Check for new messages on startup" ) );
    buttonGroup1->setTitle( tr2i18n( "Fax conversion" ) );
    kcfg_NormalQuality->setText( tr2i18n( "Normal quality (slow)" ) );
    kcfg_GoodQuality->setText( tr2i18n( "Good quality (very slow)" ) );
}

#include "GeneralWidget.moc"
