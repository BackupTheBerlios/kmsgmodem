/***************************************************************************
 *   Copyright (C) 2004 by Alexander Wiedenbruch                           *
 *   wirr@users.berlios.de                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include "modemsettingsdialog.h"

#include <klineedit.h>
#include <knuminput.h>
#include <kmessagebox.h>
#include <klocale.h>

#include <qregexp.h>
#include <qcheckbox.h>

ModemSettingsDialog::ModemSettingsDialog(UsrSmpThread *modem, QWidget* parent, const char* name, bool modal, WFlags fl)
: MyModemSettings(parent,name, modal,fl)
{
	if(modem->running())
	{
		return;
	}
	
	this->modem = modem;
	
	QRegExp FaxExp("[\x20-\x60\x7B-\x7E]{0,20}");	// ASCII 20h-60h and 7Bh-7Eh, max 20 chars
	FaxValid = new QRegExpValidator(FaxExp, this);
	faxId->setValidator(FaxValid);
	faxIdOrg = modem->GetFaxId();
	faxId->setText(faxIdOrg);
	
	rings = modem->GetRingSetup();
	Rings->setValue(rings);
	
	dialupStatus = modem->GetDailupRetrivalStatus();
	Dialup->setChecked(dialupStatus);
	
	QRegExp PwdExp("[\x30-\x39]{4,4}");
	PwdValid = new QRegExpValidator(PwdExp, this);
	Pass->setValidator(PwdValid);
	password = modem->GetDialupPassword();
	Pass->setText(password);
	
	voiceStatus = modem->GetVoiceReceptionStatus();
	Voice->setChecked(voiceStatus);
	
	faxStatus = modem->GetFaxReceptionStatus();
	Fax->setChecked(faxStatus);
	
	settingsChangedVar = false;
}


ModemSettingsDialog::~ModemSettingsDialog()
{
	delete FaxValid;
	delete PwdValid;
}


void ModemSettingsDialog::WriteSettings()
{
	if(!settingsChangedVar) return;
	
	int rc = KMessageBox::warningYesNo(this, i18n("Are you sure to save the settings?"));
	if(rc == KMessageBox::No) return;
	
	if(faxIdOrg != faxId->text())
		modem->SetFaxId(faxId->text());
		
	if(rings != Rings->value())
		modem->SetRingSetup(Rings->value());
		
	if(dialupStatus != Dialup->isChecked())
		modem->SetDailupRetrival(Dialup->isChecked());
		
	if(password != Pass->text())
		modem->SetDialupPassword(Pass->text());
		
	if(faxStatus != Fax->isChecked())
		modem->SetFaxReceptionStatus(Fax->isChecked());
		
	if(voiceStatus != Voice->isChecked())
		modem->SetVoiceReceptionStatus(Voice->isChecked());
		
	int error = modem->WriteSettings();
	if(error) KMessageBox::error(this, i18n("The settings could not be saved."));
}


void ModemSettingsDialog::settingsChanged()
{
	settingsChangedVar = true;
}



#include "modemsettingsdialog.moc"

