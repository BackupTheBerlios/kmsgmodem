/***************************************************************************
 *   Copyright (C) 2004 by Alexander Wiedenbruch                           *
 *   wirr@abacho.de                                                        *
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
/*
 * Copyright (C) 2004 Alexander Wiedenbruch <wirr@abacho.de>
 */

#include <time.h>

#include <kmainwindow.h>
#include <klocale.h>
#include <klistview.h>
#include <qtimer.h>
#include <kmessagebox.h>
#include <kapplication.h>
#include <kconfigdialog.h>
#include <kdebug.h>
#include <kfiledialog.h>

#include "kmsgmodem.h"
#include "kmsgmodemwidget.h"
#include "generalsettings.h"
#include "modemsettings.h"
#include "usrsmpthread.h"



/*! \brief C'tor
 *
 */
KMsgModem::KMsgModem(KUniqueApplication *app)
    : KMainWindow( 0, "KMsgModem" )
{
	//
	// Assemble GUI
	//
	saveas = KStdAction::saveAs(this, SLOT(SaveFile()), actionCollection(), "save_as");
	saveas->setEnabled(false);
	
	KStdAction::quit(app, SLOT(quit()), actionCollection());
	
	reload = new KAction(i18n("&Load messages"), "reload", 0, this, SLOT(LoadMessages()),  actionCollection(), "load_messages");
	reload->setEnabled(false);
	
	standaloneToogle = new KAction(i18n("&Unset standalone mode"),  "connect_established", 0, this, SLOT(ToogleStandaloneMode()),  
										actionCollection(), "toogle_standalonemode");
	standaloneToogle->setEnabled(false);
	
	stop = new KAction(i18n("&Stop playing voice message"),  "player_stop", 0, this, SLOT(StopPlayingVoice()),  actionCollection(), "stop_voice");
	stop->setEnabled(false);
	
	clearmemory = new KAction(i18n("&Clear modem memory"),  "edittrash", 0, this, SLOT(ClearMemory()),  actionCollection(), "clear_mem");
	clearmemory->setEnabled(false);
	
	KStdAction::preferences(this, SLOT(showSettings()), actionCollection());
  
	mainWidget = new KMsgModemWidget(this);
	setCentralWidget(mainWidget);
	
	statusbar = new KStatusBar(this, "statusbar");
	statusbar->insertItem(i18n("Starting..."), 0, true);
	statusbar->insertItem(i18n("No new messages"), 1, true);
	statusbar->insertItem(i18n("Mem: unknown"), 2, true);
	
	createGUI();
	
	//
	// the rest
	//
	application = app;
	
	modem = NULL;
	standaloneModeActive = true;
	
	dispatcher = new KArtsDispatcher;
 	server = new KArtsServer;
	playobj = NULL;

	selectedMessage = 0;
}


/*! \brief D'tor
 *
 */
KMsgModem::~KMsgModem()
{	
	Config *settings = Config::Self();
	
	if(settings->SetStandAloneModeOnExit && !standaloneModeActive)
	{
		ToogleStandaloneMode();
	}

	modem->terminate();	// Is that all neccessary?
	modem->exit();
	modem->wait();	// Wait so that the programm dont crash
	delete modem;
	modem = NULL;
	
	delete Config::Self();
	
	StopPlayingVoice();	// @todo why didn't it stop playing here?
	
	delete playobj;
	delete dispatcher;
	delete server;
}


/*! \brief This is the main startup routine
 *
 *	In this function we try to establish a connection to the modem,
 *	and finally start loading messages.
 *  
 */
void KMsgModem::Startup()
{
	// Read Config
	Config *settings = Config::Self();
	bool stopFunc = true;
	
	do
	{
		if(modem == NULL)
		{
			//
			// We init the modem here, if we don't get a response the timeout
			// rescues the program.
			//
			modem = new UsrSmpThread(settings->Port, settings->Baudrate);
		}
		
		modem->InitModem();
		
		bool timeoutO = false;
		time_t timeout = time(NULL) + 5;
		
		while(true)
		{
			application->processEvents();
			if(timeout < time(NULL))
			{ 
				timeoutO = true;
				break;
			}
			if(modem->finished()) break;
		}
		
		//
		// Show some error messages
		//
		if(timeoutO)
		{
			//
			// Ok, we got a timeout. For the used that means that he should look if the modem
			// is connected to the correct port or change the port in the preferences.
			// This means we need a new UsrSmpThread.
			//
			KMessageBox::error(this, i18n("Please check if the modem is connected to the correct port and switched on."));
			modem->terminate();
			modem->wait();
			delete modem;
			modem = NULL;
			stopFunc = false;
			continue;
		}
		
		int error = modem->GetInitError();
		
		if(error == RESET_ERROR)
		{
			//
			// Same as in timeout
			//
			KMessageBox::error(this, i18n("Please check if the modem is connected to the correct port."));
			modem->terminate();
			modem->wait();
			delete modem;
			modem = NULL;
			stopFunc = false;
			continue;
		}
		
		if(error == UNSUP)
		{
			//
			// We can't rescue the program in this case
			//
			KMessageBox::error(this, i18n("Please connect a USR Sportster Message Plus or Professional modem."));
			return;
		}
		
		if(error != NONE)
		{
			reload->setEnabled(true);
			standaloneToogle->setEnabled(true);
			stop->setEnabled(false);
			clearmemory->setEnabled(true);
			stopFunc = true;
			break;
		}
	}
	while(stopFunc != true);
	
	SetStatusbarText();
	if(settings->CheckOnStart) LoadMessages();
}


/*! \brief This function loads the messages from the modem
 *
 *  This dfunction loads the messages from the modem and
 *	insert them into the list.
 */
void KMsgModem::LoadMessages()
{
	SetStatusbarText(i18n("Loading messages..."));
	if(standaloneModeActive == false) ToogleStandaloneMode();
	
	modem->ReadMemoryInfo();
	
	while(!modem->finished()) application->processEvents();
	
	struct MemoryInfo MemInfo = modem->GetMemoryInfo();
	
	// There are new messages
	if((MemInfo.UnreleasedFaxMsgs > 0) || (MemInfo.UnreleasedVoiceMsgs > 0))
	{
		NewMessage();
	}
	
	// The memory file is not up to date, e.g. the modem memory was changed by another program
	Config *settings = Config::Self();
	if((MemInfo.FaxMsgs != settings->NoOfFaxMsgs) || (MemInfo.VoiceMsgs != settings->NoOfVoiceMsgs))
	{
		SetStatusbarText(i18n("Downloading messages..."));
		
		modem->LoadMessages();
		while(!modem->finished()) application->processEvents();
		
		settings->NoOfFaxMsgs = MemInfo.FaxMsgs;
		settings->NoOfVoiceMsgs = MemInfo.VoiceMsgs;
		settings->writeConfig();
	}
	
	// The temporary file was deleted, so we must read the messages again
	FILE *fileEx = fopen("/tmp/USRMemory", "r");
	if(fileEx == NULL)
	{
		SetStatusbarText(i18n("Downloading messages..."));
		
		modem->LoadMessages();
		while(!modem->finished()) application->processEvents();
		
		settings->NoOfFaxMsgs = MemInfo.FaxMsgs;
		settings->NoOfVoiceMsgs = MemInfo.VoiceMsgs;
		settings->writeConfig();
	}
	if(fileEx != NULL) fclose(fileEx);
	
	QPtrList<struct MessageInfo> MsgInfo;
	
	MsgInfo = modem->GetMsgInfo();
	
	mainWidget->MessageView->clear();
	
	// Put messages into the list
	for(int Message = 0; Message < MemInfo.StoredMsgs; Message++)
	{
		QString Number("%1");
		Number = Number.arg(MsgInfo.at(Message)->MsgNumber);
		if(MsgInfo.at(Message)->MsgNumber < 10) Number.insert(0, "0");
		
		QString Type;
		if(MsgInfo.at(Message)->MsgType == 1) Type = i18n("Fax");
		else Type = i18n("Tel");
		
		QString Date;
		/*
		if(MsgInfo.at(Message)->ClockValid)
		{
			// @todo format date
			Date = "%1:%2:%3";
			Date = Date.arg(MsgInfo.at(Message)->ReceiveDay).arg(MsgInfo.at(Message)->ReceiveHour)
															.arg(MsgInfo.at(Message)->ReceiveMinute);
		}
		else 
		*/
		Date = i18n("unknown");
			
		QString Size("%1");
		Size = Size.arg(MsgInfo.at(Message)->MsgSize);
		if(MsgInfo.at(Message)->MsgType == 1)
		{
			if(MsgInfo.at(Message)->MsgType > 1) Size += i18n(" pages");
			else Size += i18n(" page");
		}
		else Size += i18n("s");
		
		QString Caller = MsgInfo.at(Message)->CallerId.simplifyWhiteSpace();
		if(Caller.isEmpty()) Caller = i18n("unknown");
		
		new KListViewItem(mainWidget->MessageView, Number, Type, Date, Size, Caller);
	}
	
	QString memory(i18n("Mem: %1% free"));
	memory = memory.arg(100-MemInfo.MemUsed);
	
	SetStatusbarText(memory, 2);
	
	SetStatusbarText();
}


/*! \brief If a new message is received this function does everything neccessary
 *
 */
void KMsgModem::NewMessage()
{
	struct MemoryInfo MemInfo = modem->GetMemoryInfo();
	
	int newMessages = MemInfo.UnreleasedFaxMsgs + MemInfo.UnreleasedVoiceMsgs;
	
	QString newMsgText(i18n("%1 new message"));
	newMsgText = newMsgText.arg(newMessages);
	
	if(newMessages > 1) newMsgText += "s";
	
	SetStatusbarText(newMsgText, 1, false);
	
	SetStatusbarText(i18n("Downloading new messages..."));
	
	modem->LoadMessages();
	while(!modem->finished()) application->processEvents();
	
	SetStatusbarText(i18n("Idle"));
	
	//
	// Save the message numbers for the case another programm downloaded
	// the new messages first.
	//
	Config *settings = Config::Self();
	settings->NoOfFaxMsgs = MemInfo.FaxMsgs;
	settings->NoOfVoiceMsgs = MemInfo.VoiceMsgs;
	settings->writeConfig();
	
	KMessageBox::information(this, i18n("There are new messages"));
}


/*! \brief This function is called when a fax should be shown
 *
 */
void KMsgModem::ShowFax(int Fax)
{
	selectedMessage = Fax;
	
	//
	// Normally GetMessage should return the correct number of pages,
	// but i could find a easy way to do this.
	//
	MessageInfo info = modem->GetMsgInfo(Fax);
	
	if(info.MsgSize == 0)
	{
		KMessageBox::error(this, i18n("This fax has no pages.\nThis usually happens when an error occured at the \
										transmission of the fax."));
		return;
	}
	
	SetStatusbarText(i18n("Showing fax message...", 0));
	
	saveas->setEnabled(false);
	
	int Pages = modem->GetMessage(Fax);


	QStringList arguments("-normal");
	
	for(int i = 1; i <= Pages; i++)
	{
		QString faxarg("/tmp/fax-%1");
		arguments += faxarg.arg(i);
	}
	
	int rc = KApplication::kdeinitExecWait("kfax", arguments);
	
	if(rc != 0) 
	{
		KMessageBox::error(this, i18n("Please install KFax.\nYou can find the sourcecode at http:/www.kde.org/"));
	}

	/*
	//
	// This a possible option, but we must kill every running
	// kfax when quitting.
	//
	KProcess *proc = new KProcess;

	*proc << "kfax";
	*proc << "-normal";
	for(int i = 1; i <= Pages; i++)
	{
		QString faxarg("/tmp/fax-%1");
		*proc << faxarg.arg(i);
	}
	
	bool rc = proc->start();
	if(!rc) 
	{
		KMessageBox::error(this, i18n("Please install KFax.\nYou can find the sourcecode at http:/www.kde.org/"));
	}
	
	while(proc->isRunning()) application->processEvents();
	*/
	
	for(int i = 1; i <= Pages; i++)
	{
		QString faxarg("/tmp/fax-%1");
		faxarg = faxarg.arg(i);
		
		remove(faxarg);
	}
	
	SetStatusbarText();
}


/*! \brief This function plays the voice files
 *
 */
void KMsgModem::PlayVoice(int Voice)
{	
	selectedMessage = Voice;

	if(playobj != NULL) StopPlayingVoice();
	
	saveas->setEnabled(true);
	
	modem->GetMessage(Voice);

	SetStatusbarText(i18n("Playing voice message..."));
		
	//
	// Setting attributes for sox and run it. 
	// It is pretty fast, so we don't get asynchron.
	// Command: sox /tmp/tel.gsm -u -b -c 1 /tmp/tel.wav
	//
	KProcess *proc = new KProcess;

	*proc << "sox";
	*proc << "/tmp/tel.gsm" << "-u" << "-b" << "-c 1" << "/tmp/tel.wav";
	
	bool rc = proc->start(KProcess::Block);
	
	remove("/tmp/tel.gsm");
	
	if(!rc) 
	{
		KMessageBox::error(this, i18n("The voice file could not be decoded.\nPlease install SoX. You can find the sourcecode at \
										http://sox.sf.net/"));
		return;
	}

	//
	// Now play the voice file
	// @todo How can i check if an artsd is currently running?
	//
	KURL url("/tmp/tel.wav");
	
 	KDE::PlayObjectFactory factory(server->server());
 	playobj = factory.createPlayObject(url, true);
 	playobj->play();
	
	stop->setEnabled(true);
	
	while(playobj->state() == Arts::posPlaying) application->processEvents();
		
	//
	// clean up
	//
	remove("/tmp/tel.wav");

	stop->setEnabled(false);
	
	SetStatusbarText();
}


/*! \brief This function shows the settings
 *
 */
void KMsgModem::showSettings()
{
   if(KConfigDialog::showDialog("settings"))
     return;
   
   KConfigDialog *dialog = new KConfigDialog(this, "settings", Config::Self(), 	// bad, change when introducing singelton config
   								KDialogBase::IconList, KDialogBase::Ok|KDialogBase::Apply|KDialogBase::Cancel, KDialogBase::Ok, true);
   
   dialog->addPage(new GeneralSettings(0, "General"), i18n("General"), "kmsgmodem", i18n("General Settings"));
   
   dialog->addPage(new ModemConnectionSettings(0, "Connection"), i18n("Connection"), "connect_no",  i18n("Connection Settings"));
   
   dialog->show();
}


/*! \brief This function toogles the standalone mode
 *
 */
void KMsgModem::ToogleStandaloneMode()
{
	if(standaloneModeActive)
	{
		standaloneModeActive = false;
		standaloneToogle->setText(i18n("Set standalone mode"));
		standaloneToogle->setIcon("connect_no");
		int rc = modem->UnSetStandAloneMode();
		if(rc == 0) SetStatusbarText(i18n("Standalone mode unset"), 0, true);
	}
	else
	{
		standaloneModeActive = true;
		standaloneToogle->setText(i18n("Unset standalone mode"));
		standaloneToogle->setIcon("connect_established");
		int rc = modem->SetStandAloneMode();
		if(rc == 0) SetStatusbarText(i18n("Standalone mode set"), 0, true);
	}
}


/*! \brief This function display the text in the specified statusbar
 *
 */
void KMsgModem::SetStatusbarText(QString text, int item, bool WithTimeout)
{
	statusbar->changeItem(text, item);
	
	if(WithTimeout)
	{
		QTimer::singleShot(3000, this, SLOT(SetStatusbarText()));
	}
}


/*! \brief This function kills the playing voice full
 *	
 */
void KMsgModem::StopPlayingVoice()
{
	playobj->halt();
	
	stop->setEnabled(false);
}


/*! \brief This function clears the memory of the modem
 *
 *	untested
 */
void KMsgModem::ClearMemory()
{
	int rc = KMessageBox::warningYesNo(this, i18n("Are you sure to erase all messages on the modem?"));
	
	if(rc == KMessageBox::Yes)
	{
		modem->ClearMemory();
		while(!modem->finished()) application->processEvents();
		LoadMessages();
	}
}


/*! \brief This function saves the voice file
 *
 *	Saving of fax files is not supported atm.
 */
void KMsgModem::SaveFile()
{
	//
	// I think the user want to save the current selected item
	// and not the playing one.
	//

	QString filename = KFileDialog::getSaveFileName("voice-message.wav", "audio/x-wav", this);
	if((filename.isEmpty()))
	{
		return;
	}

	modem->GetMessage(selectedMessage);
	
	//
	// Now convert the gsm file and save it to the location
	//
	KProcess *proc = new KProcess;

	*proc << "sox";
	*proc << "/tmp/tel.gsm" << "-u" << "-b" << "-c 1" << filename;
	
	bool rc = proc->start(KProcess::Block);
	
	remove("/tmp/tel.gsm");
	
	if(!rc) 
	{
		KMessageBox::error(this, i18n("The voice file could not be decoded.\nPlease install SoX. You can find the sourcecode at \
										http://sox.sf.net/"));
		return;
	}
}

#include "kmsgmodem.moc"
