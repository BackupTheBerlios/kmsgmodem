#include <kdialog.h>
#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './ModemSettings.ui'
**
** Created: Sat Oct 16 15:06:51 2004
**      by: The User Interface Compiler ($Id: ModemSettings.cpp,v 1.6 2004/10/16 13:25:03 wirr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "ModemSettings.h"

#include <qvariant.h>
#include <qframe.h>
#include <qlabel.h>
#include <qgroupbox.h>
#include <knuminput.h>
#include <qcheckbox.h>
#include <klineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a MyModemSettings as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
MyModemSettings::MyModemSettings( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "MyModemSettings" );
    setMinimumSize( QSize( 325, 450 ) );

    line1 = new QFrame( this, "line1" );
    line1->setGeometry( QRect( 10, 10, 300, 20 ) );
    line1->setFrameShape( QFrame::HLine );
    line1->setFrameShadow( QFrame::Sunken );
    line1->setLineWidth( 1 );
    line1->setFrameShape( QFrame::HLine );

    textLabel1 = new QLabel( this, "textLabel1" );
    textLabel1->setGeometry( QRect( 10, 0, 250, 16 ) );
    QFont textLabel1_font(  textLabel1->font() );
    textLabel1_font.setBold( TRUE );
    textLabel1->setFont( textLabel1_font ); 

    groupBox5 = new QGroupBox( this, "groupBox5" );
    groupBox5->setGeometry( QRect( 10, 130, 300, 70 ) );

    textLabel1_2 = new QLabel( groupBox5, "textLabel1_2" );
    textLabel1_2->setGeometry( QRect( 20, 30, 140, 20 ) );
    textLabel1_2->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignLeft ) );

    Rings = new KIntSpinBox( groupBox5, "Rings" );
    Rings->setGeometry( QRect( 230, 30, 50, 22 ) );
    Rings->setMaxValue( 6 );
    Rings->setMinValue( 3 );

    groupBox7 = new QGroupBox( this, "groupBox7" );
    groupBox7->setGeometry( QRect( 10, 340, 300, 100 ) );

    Fax = new QCheckBox( groupBox7, "Fax" );
    Fax->setGeometry( QRect( 20, 30, 190, 20 ) );

    Voice = new QCheckBox( groupBox7, "Voice" );
    Voice->setGeometry( QRect( 20, 60, 210, 20 ) );

    groupBox4 = new QGroupBox( this, "groupBox4" );
    groupBox4->setGeometry( QRect( 10, 40, 300, 70 ) );

    faxIdLabel = new QLabel( groupBox4, "faxIdLabel" );
    faxIdLabel->setGeometry( QRect( 20, 30, 43, 22 ) );
    faxIdLabel->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignLeft ) );

    faxId = new KLineEdit( groupBox4, "faxId" );
    faxId->setGeometry( QRect( 120, 30, 160, 21 ) );
    faxId->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, faxId->sizePolicy().hasHeightForWidth() ) );
    faxId->setMinimumSize( QSize( 160, 21 ) );

    groupBox6 = new QGroupBox( this, "groupBox6" );
    groupBox6->setGeometry( QRect( 10, 220, 300, 100 ) );

    textLabel2 = new QLabel( groupBox6, "textLabel2" );
    textLabel2->setGeometry( QRect( 130, 60, 65, 20 ) );

    Pass = new KLineEdit( groupBox6, "Pass" );
    Pass->setEnabled( TRUE );
    Pass->setGeometry( QRect( 240, 60, 40, 22 ) );

    Dialup = new QCheckBox( groupBox6, "Dialup" );
    Dialup->setGeometry( QRect( 20, 30, 190, 20 ) );
    languageChange();
    resize( QSize(325, 454).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( faxId, SIGNAL( textChanged(const QString&) ), this, SLOT( settingsChanged() ) );
    connect( Rings, SIGNAL( valueChanged(int) ), this, SLOT( settingsChanged() ) );
    connect( Dialup, SIGNAL( toggled(bool) ), this, SLOT( settingsChanged() ) );
    connect( Pass, SIGNAL( textChanged(const QString&) ), this, SLOT( settingsChanged() ) );
    connect( Voice, SIGNAL( toggled(bool) ), this, SLOT( settingsChanged() ) );
    connect( Fax, SIGNAL( toggled(bool) ), this, SLOT( settingsChanged() ) );

    // tab order
    setTabOrder( faxId, Rings );
    setTabOrder( Rings, Dialup );
    setTabOrder( Dialup, Pass );
    setTabOrder( Pass, Fax );
    setTabOrder( Fax, Voice );
}

/*
 *  Destroys the object and frees any allocated resources
 */
MyModemSettings::~MyModemSettings()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void MyModemSettings::languageChange()
{
    setCaption( tr2i18n( "Form1" ) );
    textLabel1->setText( tr2i18n( "Modem Settings" ) );
    groupBox5->setTitle( tr2i18n( "Rings before pick up" ) );
    textLabel1_2->setText( tr2i18n( "Number of rings:" ) );
    groupBox7->setTitle( tr2i18n( "Message reception" ) );
    Fax->setText( tr2i18n( "Enable fax reception" ) );
    Voice->setText( tr2i18n( "Enable voice reception" ) );
    groupBox4->setTitle( tr2i18n( "Fax identification string" ) );
    faxIdLabel->setText( tr2i18n( "Fax ID:" ) );
    groupBox6->setTitle( tr2i18n( "Dialup retrieval" ) );
    textLabel2->setText( tr2i18n( "Password:" ) );
    Dialup->setText( tr2i18n( "Enable dialup retrieval" ) );
}

void MyModemSettings::settingsChanged()
{
    qWarning( "MyModemSettings::settingsChanged(): Not implemented yet" );
}

void MyModemSettings::Fax_stateChanged(int)
{
    qWarning( "MyModemSettings::Fax_stateChanged(int): Not implemented yet" );
}

#include "ModemSettings.moc"
