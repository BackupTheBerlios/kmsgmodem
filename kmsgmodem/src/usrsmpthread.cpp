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
#include <stdlib.h>

#include <kdebug.h>

#include "usrsmpthread.h"


/*! \brief C'tor
 *
 *  Not much in here
 */
UsrSmpThread::UsrSmpThread(QString interface, QString baud)
 : QThread()
{
	simplemodem = new SimpleModem(interface, baud);
	
	if(simplemodem->GetConnection() == NULL) initError = INIT_ERROR;
	else initError = NONE;
	
	funcToRun = DEF;
	
	WasDLE = 0;
}


/*! \brief D'tor
 *
 *  Not much in here
 */
UsrSmpThread::~UsrSmpThread()
{
	delete simplemodem;
}


/*! \brief This is the function which runs in the new thread
 *

 */
void UsrSmpThread::run()
{
	switch(funcToRun)
	{
		case INIT:	InitModemThread();
					break;
					
		case READMEMORY:
					ReadMemoryInfoThread();
					break;
					
		case LOADMESSAGES:
					LoadMessagesThread();
					break;
					
		case PLAYVOICE:
					PlayVoiceFileThread();
					break;
					
		case ERASEMESSAGES:
					ClearMemoryThread();
					break;
					
		default:	break;
	}
}


/*! \brief This function inits the modem
 *
 *	Look in InitModemPrivate for the code
 */
void UsrSmpThread::InitModem()
{
	funcToRun = INIT;
	
	start();
}


/*! \brief Gets memory and message informations from the modem
 *
 *  This function gets information about the memory first.
 *  After that it gets the information about all stored messages
 *
 *  Look inReadMemoryInfoPrivate for the code
 */
void UsrSmpThread::ReadMemoryInfo()
{
	funcToRun = READMEMORY;
	
	start();
}


/*! \brief This function load the stored messages to a temporary file
 *
 *	Look in LoadMessagesPrivate for the code
 */
void UsrSmpThread::LoadMessages()
{
	funcToRun = LOADMESSAGES;
	
	start();
}


/*! \brief Plays the voice file
 *
 */
void UsrSmpThread::PlayVoiceFile()
{
	funcToRun = PLAYVOICE;
	
	start();
}


/*! \brief Deletes all messages
 *
  *	Look in ClearMemoryPrivate for the code
 */
void UsrSmpThread::ClearMemory()
{
	funcToRun = ERASEMESSAGES;
	
	start();
}

/*! \brief This function inits the modem
 *
 */
int UsrSmpThread::InitModemThread()
{	
	
	if(initError != NONE) return 0;
	
	//  In +M mode this resets the flash memory
	/*
	QString resp = simplemodem->SendCommand("ATZ");
	if(resp.find("OK") == -1)
	{
		initError = RESET_ERROR;
		return 0;
	}
	*/
	
	modemType = DetectModemType();
	if(modemType == UNSPECIFIED)
	{
		initError = UNSUP;
		return 0;
	}	
	
	SetStandAloneMode();
	
	initError = NONE;
	
	return 1;
}


/*! \brief Detects if we have a USR modem connected
 *
 *  This function sends ATI3 to the modem, receives the
 *  modem information and decides what type of modem is connected
 *
 *  Partly (C) T. Uhlmann from usrmodem.cpp
 */
int UsrSmpThread::DetectModemType()
{
	QString resp = simplemodem->SendCommand("ATI3");
	
	if ((!resp.contains("message",false)) && (!resp.contains("msg",false)))
	{
		kdDebug() << "Neither Message nor Message Professional: " << resp << endl;
		return UNSPECIFIED;
	}
	if (resp.contains("pro",false))
	{
		kdDebug() << "Professional Message Modem: " << resp << endl;
		return PROFESSIONAL;
	}
	
	kdDebug() << "Normal Message Modem: " << resp << endl;
	return NORMAL;
}


/*! \brief Returns the error wich happens in init
 *
 */
int UsrSmpThread::GetInitError()
{
	return initError;
}


/*! \brief Gets memory and message informations from the modem
 *
 *  This function gets information about the memory first.
 *  After that it gets the information about all stored messages
 *
 *  Partly (C) T. Uhlmann from usrmodem.cpp
 */
int UsrSmpThread::ReadMemoryInfoThread()
{
	//
	// Get an overview of the stored messages
	//
	QString MemInfoString = simplemodem->SendCommand("AT+MSR=0");
	
	if(MemInfoString.find("OK") == -1)
	{
		kdError(0) << "Couldn' get memory information" << endl;
		return 1;
	}
	
	MemInfoString = MemInfoString.remove("\n");
	MemInfoString = MemInfoString.remove("\r");
	MemInfoString = MemInfoString.remove("OK");
	
	kdDebug() << "Memory info string: " << MemInfoString << endl;
			
	MemInfo.MemSize = MemInfoString.mid(0,3).toInt();
	
	MemInfo.MemUsed = MemInfoString.mid(4,3).toInt();
	
	MemInfo.VoiceMsgs = MemInfoString.mid(8,3).toInt();
	
	MemInfo.UnreleasedVoiceMsgs = MemInfoString.mid(12,3).toInt();
	
	MemInfo.FaxMsgs = MemInfoString.mid(16,3).toInt();
	
	MemInfo.UnreleasedFaxMsgs = MemInfoString.mid(20,3).toInt();
	
	// rest skipped
	
	MemInfo.StoredMsgs = MemInfo.VoiceMsgs + MemInfo.FaxMsgs;
	
	#ifdef DEBUG_USR
		kdDebug()  << "\nMemory Size:\t\t" << MemInfo.MemSize
		  			<< "\nMemory Used:\t\t" << MemInfo.MemUsed
					<< "\nVoice Messages:\t\t" << MemInfo.VoiceMsgs
					<< "\nUnreleased Voice Messages:\t\t" << MemInfo.UnreleasedVoiceMsgs
		  			<< "\nFax Messages:\t\t" << MemInfo.FaxMsgs
					<< "\nUnreleased Fax Messages:\t\t" << MemInfo.UnreleasedFaxMsgs
		 			<< endl;
	#endif
	
	//
	// Get infos about messages
	//	
	MsgInfoList.setAutoDelete(true);
	MsgInfoList.clear();
			
	for(int message = 1; message <= MemInfo.StoredMsgs; message++)
	{
		QString Command("AT+MSR=%1");
		Command = Command.arg(message);
		
		QString MsgInfoString = simplemodem->SendCommand(Command);
		
		if(MsgInfoString.find("OK") == -1)
		{
			kdError(0) << "Couldn' get information about message " << message << endl;
			return 2;
		}
		
		MsgInfoString = MsgInfoString.remove("\n");
		MsgInfoString = MsgInfoString.remove("\r");
		MsgInfoString = MsgInfoString.remove("OK");
	
		#ifdef DEBUG_USR
			kdDebug() << "Message info string: " << MsgInfoString << endl;
		#endif
				
		MessageInfo *MsgInfo = new MessageInfo;
				
		MsgInfo->MsgNumber = MsgInfoString.mid(0,3).toInt();
		
		MsgInfo->MsgType = MsgInfoString.mid(4,3).toInt();
		
		MsgInfo->MsgSize = MsgInfoString.mid(8               ,3).toInt();
		
		switch (MsgInfoString.mid(12,3).toInt())
		{
			case 255: 	MsgInfo->MsgAttribute = 1 /*UnreleasedUnchecked*/;
						break;
						
			case 253: 	MsgInfo->MsgAttribute = 2 /*UnreleasedChecked*/;
						break;
						
			case 252: 	MsgInfo->MsgAttribute = 3 /*ReleasedChecked*/;
						break;
						
			default: 	MsgInfo->MsgAttribute = 4 /*Unknown*/;
		}
		
		MsgInfo->MsgStatus = MsgInfoString.mid(16,3).toInt();
		
		if (MsgInfoString.mid(20,3).toInt() == 255)
		{
			MsgInfo->ClockValid = false;
			
		} else {
			MsgInfo->ClockValid = true;
			MsgInfo->ReceiveDay = MsgInfoString.mid(20,3).toInt();
			MsgInfo->ReceiveHour = MsgInfoString.mid(24,3).toInt();
			MsgInfo->ReceiveMinute = MsgInfoString.mid(28,3).toInt();
		}
		
		MsgInfo->CallerId = MsgInfoString.mid(32,20).copy();
		MsgInfo->CallerId = MsgInfo->CallerId.stripWhiteSpace();	
		//if(MsgInfo->CallerId.isEmpty()) MsgInfo->CallerId = "";
		
		MsgInfo->MemStartPage = MsgInfoString.mid(53,3).toInt();
		MsgInfo->MemAddress = MsgInfoString.mid(57,3).toInt();
		MsgInfo->MemAddress = MsgInfo->MemAddress << 8;
		MsgInfo->MemAddress += MsgInfoString.mid(61,3).toInt();
		
		MsgInfoList.append(MsgInfo);
		
		#ifdef DEBUG_USR
			if(MsgInfo->ClockValid)
			{
				kdDebug()  << "\nMessage Number:\t\t" << MsgInfo->MsgNumber
							<< "\nMessage Type:\t\t" << MsgInfo->MsgType
							<< "\nMessage Size:\t\t" << MsgInfo->MsgSize
							<< "\nMessage Attribute:\t\t" << MsgInfo->MsgAttribute
							<< "\nReceive Day: \t\t" << MsgInfo->ReceiveDay
							<< "\nReceive Hour:\t\t" << MsgInfo->ReceiveHour
							<< "\nReceive Minute:\t\t" << MsgInfo->ReceiveMinute
							<< "\nCaller Id:\t\t" << MsgInfo->CallerId
							<< endl;
			}
			else
			{
				kdDebug()  << "\nMessage Number:\t\t" << MsgInfo->MsgNumber
							<< "\nMessage Type:\t\t" << MsgInfo->MsgType
							<< "\nMessage Attribute:\t\t" << MsgInfo->MsgAttribute
							<< "\nCaller Id:\t\t" << MsgInfo->CallerId
							<< endl;
			}
		#endif
	}

	return 0;
}


/*! \brief Returns information about the memory
 *
 */
struct MemoryInfo UsrSmpThread::GetMemoryInfo()
{
	return MemInfo;
}


/*! \brief Returns information about all messages
 *
 */
QPtrList<struct MessageInfo> UsrSmpThread::GetMsgInfo()
{
	return MsgInfoList;
}


/*! \brief Returns information about the specified Message
 *
 */
struct MessageInfo UsrSmpThread::GetMsgInfo(unsigned int index)
{	
	struct MessageInfo ret;
	
	if(index == 0)
	{
		kdError(0) << "Message index out of range" << endl;
		ret.MsgNumber = 0;
		return ret;
	}
		
	ret = *(MsgInfoList.at(index-1));
	return ret;
}


/*! \brief This function load the stored messages to a temporary file
 *
 */
void UsrSmpThread::LoadMessagesThread()
{
	FILE *fd = fopen("/tmp/USRMemory", "wb");
	if(fd == NULL)
	{
		kdError(0) << "Could not open temporary memory file" << endl;
		return;
	}
	
	simplemodem->ReadMemoryToFile(fd);
	
	fclose(fd);
}


/*! \brief This function gets the specified message from the temporary file
 *
 *  Partly (C) T. Uhlmann from usrmodem.cpp
 */
int UsrSmpThread::GetMessage(int Message)
{
	FILE *fd = fopen("/tmp/USRMemory", "rb");
	if(fd == NULL)
	{
		kdError(0) << "Could not open temporary memory file" << endl;
		return -1;
	}
	
	int Read, MsgAdr;
	bool found = false, readError = false;
	Header hdr;
	
	unsigned int PageNo = 1;
	
	while ((!found) && (!readError) && (!feof(fd)))
  	{
    	/* read header */
    	Read = fread(&hdr, 34, 1, fd);
    	if (Read == 1)
		{
      		if(hdr.Index == Message)
			{
	      		found = true;
      		} 
			else 
			{
	      		MsgAdr = hdr.NextAddr1 * 256;
        		MsgAdr += hdr.NextAddr2;
	      		/* add pages */
	      		MsgAdr += (hdr.NextPage - 4) * MODEM_USR_PAGE;
	      		fseek(fd, MsgAdr, SEEK_SET);
      		}
    	} 
		else 
		{
      		readError = true;
    	}
  	}

 	if(found)
	{
		kdDebug() << "Message found: " << (int)hdr.Index << endl;
				
		MsgAdr = hdr.NextAddr1 * 256;
		MsgAdr += hdr.NextAddr2;
		/* add pages */
		MsgAdr += (hdr.NextPage - 4) * MODEM_USR_PAGE;
		int Pos = ftell(fd);
		found = false;
		
		unsigned char c;
		
		char filename[20];
		
		if(hdr.Type == 2)	// Tel
		{
			sprintf(filename, "/tmp/tel.gsm");
		}
		
		if(hdr.Type == 1)	// Fax
		{
			sprintf(filename, "/tmp/fax-1");
		}
		
		FILE *MsgFd = fopen(filename, "wb");
		if(fd == NULL)
		{
			kdError(0) << "Could not create file" << endl;
			return -1;
		}
			
		while((!found) && (!feof(fd)))
		{
			if(Pos >= MsgAdr)
			{
				found = true;
			}
			else 
			{
				fread(&c, 1, 1, fd);
				if (WriteFaxPage(MsgFd,c) == MODEM_NEW_PAGE)	// new fax page
				{
					if (hdr.Type == 1)	// Fax
					{
						if ((Pos + 1) < MsgAdr)
						{
							fclose(MsgFd);
							PageNo++;
							char filename[20];
							sprintf(filename, "/tmp/fax-%i", PageNo);
							if (!(MsgFd = fopen(filename,"wb")))
							{
								return -1;
							}
						}
					} 
					else
					break;
				}
				Pos++;
			}
		}
		fclose(MsgFd);
  	}
		
	fclose(fd);
	
	if(hdr.Type == 2)	// Tel
		return 0;
	
	if(hdr.Type == 1)	// Fax
		return PageNo;
	
	return -1;
}


/*! \brief This function splits a stored fax message into tif files
 *
 *  (C) T. Uhlmann from usrmodem.cpp
 */
int UsrSmpThread::WriteFaxPage(FILE *msgFile, unsigned char c)
{
  int EndOfPage = 0;

  if ( !WasDLE )
  {
    if ( c == MODEM_DLE ) WasDLE = 1;
    else fputc( c, msgFile );
  }
  else
  {
    if ( c == MODEM_DLE )
      fputc( c, msgFile );	/* DLE DLE -> DLE */
    else
    {
      if ( c == MODEM_SUB )				/* DLE SUB -> 2x DLE */
      {						                /* (class 2.0) */
        fputc( MODEM_DLE, msgFile ); fputc( MODEM_DLE, msgFile );
      }
      else
      {
        if ( c == MODEM_ETX ); EndOfPage = MODEM_NEW_PAGE; /* DLE ETX -> end */
      }
    }
    WasDLE = 0;
  }
  return EndOfPage;
}


/*! \brief Returns the modem type
 *
 */
int UsrSmpThread::GetModemType()
{
	return modemType;
}


/*! \brief Sets stand alone mode for a USR modem
 *
 *  Partly (C) T. Uhlmann from usrmodem.cpp
 */
int UsrSmpThread::SetStandAloneMode()
{
QString resp;
	
	switch(modemType)
	{
		case PROFESSIONAL:
				resp = simplemodem->SendCommand("AT+MCA=1");
				if(resp.find("OK") != -1) return 0;
				break;
				
		case NORMAL:
				resp = simplemodem->SendCommand("AT+MCS=1");
				if(resp.find("OK") != -1) return 0;
				break;
				
		default: break;
	}
	
	return 1;
}


/*! \brief Unsets stand alone mode for a USR modem
 *
 *  Partly (C) T. Uhlmann from usrmodem.cpp
 */
int UsrSmpThread::UnSetStandAloneMode()
{
	QString resp;
	
	switch(modemType)
	{
		case PROFESSIONAL:
				resp = simplemodem->SendCommand("AT+MCA=0");
				if(resp.find("OK") != -1) return 0;
				break;
				
		case NORMAL:
				resp = simplemodem->SendCommand("AT+MCS=0");
				if(resp.find("OK") != -1) return 0;
				break;
				
		default: break;
	}
	
	return 1;
}


/*! \brief Plays the voice file
 *
 */
void UsrSmpThread::PlayVoiceFileThread()
{
	system("tcat -d /tmp/tel.gsm > /dev/audio");
	remove("/tmp/tel.gsm");
}


/*! \brief Deletes all messages
 *
 */
int UsrSmpThread::ClearMemoryThread()
{
	QString resp = simplemodem->SendCommand("AT+MEM");
	if(resp.find("OK") != -1) return 0;

	return 1;
}


/*! \brief This dunction gets the modem time
 *
 */
time_t UsrSmpThread::GetModemClock()
{
	QString resp = simplemodem->SendCommand("AT+MCC?");
	
	resp = resp.remove("\n");
	resp = resp.remove("\r");
	resp = resp.remove("OK");
	
	#ifdef DEBUG_USR
		kdDebug() << resp << endl;
	#endif
	
	int Day = resp.mid(0,3).toInt();
	int Hour = resp.mid(4,3).toInt();
	int Minute = resp.mid(8,3).toInt();
	int Second = resp.mid(12,3).toInt();
	
	if((Day == 255) && (Hour == 255) && (Minute == 255) && (Second == 255)) return 0;
	if((Day == 254) && (Hour == 24)) return 0;
	
	time_t date = Second;
	date += Minute * 60;
	date += Hour * 60 * 60;
	date += Day * 24 * 60 * 60;
	
	return date;
}


/*! \brief Resets the modem time
 *
 */
int UsrSmpThread::ResetModemClock()
{
	QString resp = simplemodem->SendCommand("AT+MCC");
	if(resp.find("OK") != -1) return 0;

	return 1;
}


/*! \brief Gets the current fax ID from the modem
 *
 */
QString UsrSmpThread::GetFaxId()
{	
	QString resp = simplemodem->SendCommand("AT+MFI?");
	
	resp = resp.remove("\n");
	resp = resp.remove("\r");
	resp = resp.remove("OK");
	resp = resp.remove("\"");
	
	#ifdef DEBUG_USR
		kdDebug() << resp << endl;
	#endif
	
	return resp;
}


/*! \brief Sets the fax ID
 *
 */
int UsrSmpThread::SetFaxId(QString faxId)
{
	QString command("AT+MFI=\"");
	command += faxId;
	command += "\"";
	
	QString resp = simplemodem->SendCommand(command);
	if(resp.find("OK") != -1) return 0;
	
	return 1;
}


/*! \brief Gets the number of rings before answering.
 *
 */
int UsrSmpThread::GetRingSetup()
{
	QString resp = simplemodem->SendCommand("AT+MCR?");
	
	resp = resp.remove("\n");
	resp = resp.remove("\r");
	resp = resp.remove("OK");
	resp = resp.remove("\"");
	
	#ifdef DEBUG_USR
		kdDebug() << resp << endl;
	#endif
	
	return resp.toInt();
}


/*! \brief Sets the number of rings before answering.
 *
 */
int UsrSmpThread::SetRingSetup(int rings)
{
	QString command("AT+MCR=%1");
	command = command.arg(rings);
	
	QString resp = simplemodem->SendCommand(command);
	if(resp.find("OK") != -1) return 0;
	
	return 1;
}


/*! \brief Get if the dial up retrival is enabled
 *
 */
bool UsrSmpThread::GetDailupRetrivalStatus()
{
	QString resp = simplemodem->SendCommand("AT+MCD?");
	
	#ifdef DEBUG_USR
		kdDebug() << resp << endl;
	#endif
	
	if(resp.find("0") != -1) return false;
	
	return true;
}


/*! \brief Disable/enable the dial up retrival
 *
 */
int UsrSmpThread::SetDailupRetrival(bool enable)
{
	QString command;
	
	if(enable)
		command = "AT+MCD=1";
	else
		command = "AT+MCD=0";
	
	QString resp = simplemodem->SendCommand(command);
	if(resp.find("OK") != -1) return 0;
	
	return 1;
}


/*! \brief Get dialup password
 *
 */
QString UsrSmpThread::GetDialupPassword()
{
	QString resp = simplemodem->SendCommand("AT+MCP?");

	resp = resp.remove("\n");
	resp = resp.remove("\r");
	resp = resp.remove("OK");
	
	#ifdef DEBUG_USR
		kdDebug() << resp << endl;
	#endif
	
	if(resp.isEmpty()) return QString::null;	// untested, should work
	return resp;
}


/*! \brief Set dialup password
 *
 */
int UsrSmpThread::SetDialupPassword(QString password)
{
	QString command("AT+MCP=%1");
	command = command.arg(password.toInt());
	
	QString resp = simplemodem->SendCommand(command);
	if(resp.find("OK") != -1) return 0;
	
	return 1;
}


/*! \brief Write settings into non volatile memory
 *
 */
int UsrSmpThread::WriteSettings()
{
	QString resp = simplemodem->SendCommand("AT+MCW");
	if(resp.find("OK") != -1) return 0;
	
	return 1;
}


/*! \brief Get fax reception status
 *
 */
bool UsrSmpThread::GetFaxReceptionStatus()
{
	QString resp = simplemodem->SendCommand("AT+MCF?");
	
	#ifdef DEBUG_USR
		kdDebug() << resp << endl;
	#endif
	
	if(resp.find("0") != -1) return false;
	
	return true;
}


/*! \brief Set fax reception status
 *
 */
int UsrSmpThread::SetFaxReceptionStatus(bool enable)
{
	QString command;
	
	if(enable)
		command = "AT+MCF=1";
	else
		command = "AT+MCF=0";
	
	QString resp = simplemodem->SendCommand(command);
	if(resp.find("OK") != -1) return 0;
	
	return 1;
}


/*! \brief Get voice reception status
 *
 */
bool UsrSmpThread::GetVoiceReceptionStatus()
{
	QString resp = simplemodem->SendCommand("AT+MCV?");
	
	#ifdef DEBUG_USR
		kdDebug() << resp << endl;
	#endif
	
	if(resp.find("0") != -1) return false;
	
	return true;
}


/*! \brief Set voice reception status
 *
 */
int UsrSmpThread::SetVoiceReceptionStatus(bool enable)
{
	QString command;
	
	if(enable)
		command = "AT+MCV=1";
	else
		command = "AT+MCV=0";
	
	QString resp = simplemodem->SendCommand(command);
	if(resp.find("OK") != -1) return 0;
	
	return 1;
}
