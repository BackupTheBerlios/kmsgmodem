#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './GeneralWidget.ui'
**
** Created: Mon Aug 16 15:29:46 2004
**      by: The User Interface Compiler ($Id: GeneralWidget.cpp,v 1.2 2004/08/16 14:18:20 wirr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "GeneralWidget.h"

#include <qvariant.h>
#include <qcheckbox.h>
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
    setMinimumSize( QSize( 300, 150 ) );
    GeneralLayout = new QGridLayout( this, 1, 1, 11, 6, "GeneralLayout"); 

    kcfg_CheckOnStart = new QCheckBox( this, "kcfg_CheckOnStart" );
    kcfg_CheckOnStart->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)4, (QSizePolicy::SizeType)4, 0, 0, kcfg_CheckOnStart->sizePolicy().hasHeightForWidth() ) );

    GeneralLayout->addWidget( kcfg_CheckOnStart, 0, 0 );

    kcfg_SetStandAloneModeOnExit = new QCheckBox( this, "kcfg_SetStandAloneModeOnExit" );
    kcfg_SetStandAloneModeOnExit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)4, (QSizePolicy::SizeType)4, 0, 0, kcfg_SetStandAloneModeOnExit->sizePolicy().hasHeightForWidth() ) );

    GeneralLayout->addWidget( kcfg_SetStandAloneModeOnExit, 1, 0 );
    languageChange();
    resize( QSize(300, 154).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
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
    kcfg_CheckOnStart->setText( tr2i18n( "Check for new messages on startup" ) );
    kcfg_SetStandAloneModeOnExit->setText( tr2i18n( "Set standalone mode on exit" ) );
}

#include "GeneralWidget.moc"
