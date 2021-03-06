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
#include "usrsmp.h"

#include <stdio.h>

#include <kdebug.h>

/*! \brief C'tor
 *
 *  Not much in here atm.
 */
UsrSmp::UsrSmp(QString interface, QString baud) : SimpleModem(interface, baud)
{
	WasDLE = 0;
	initError = NONE;
	
	//todo: fehlerbehandlung
	QString resp = SendCommand("ATZ");
	if(resp.find("OK") == -1)
	{
		initError = RESET_ERROR;
		return;
	}
	
	modemType = DetectModemType();
	if(modemType == UNSPECIFIED)
	{
		initError = UNSUP;
		return;
	}		
	
	SetStandAloneMode();
}


/*! \brief D'tor
 *
 *  Not much in here atm.
 */
UsrSmp::~UsrSmp()
{
}


/*! \brief Disable Echo (test)
 *
 *  This function sends ATE0 to the modem.
 *  That command will disable the echoing
 */
void UsrSmp::SetEchoOff()
{
	SendCommand("ATE0");
}


/*! \brief Detects if we have a USR modem connected
 *
 *  This function sends ATI3 to the modem, receives the
 *  modem information and decides what type of modem is connected
 *
 *  Partly (C) T. Uhlmann from usrmodem.cpp
 */
int UsrSmp::DetectModemType()
{
	QString resp = SendCommand("ATI3");
	
	if ((!resp.contains("message",false)) && (!resp.contains("msg",false)))
	{
		#ifdef DEBUG_USR
			kdDebug(0) << "Neither Message nor Message Professional: " << resp << endl;
		#endif
		return UNSPECIFIED;
	}
	if (resp.contains("pro",false))
	{
		#ifdef DEBUG_USR
			kdDebug(0) << "Professional Message Modem: " << resp << endl;
		#endif
		return PROFESSIONAL;
	}
	
	#ifdef DEBUG_USR
		kdDebug(0) << "Normal Message Modem: " << resp << endl;
	#endif
	return NORMAL;
}


/*! \brief Returns the modem type
 *
 */
int UsrSmp::GetModemType()
{
	return modemType;
}


/*! \brief Sets stand alone mode for a USR modem
 *
 *  Partly (C) T. Uhlmann from usrmodem.cpp
 */
int UsrSmp::SetStandAloneMode()
{
	QString resp;
	
	switch(modemType)
	{
		case PROFESSIONAL:
				resp = SendCommand("AT+MCA=1");
				if(resp.find("OK") != -1) return 0;
				break;
				
		case NORMAL:
				resp = SendCommand("AT+MCS=1");
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
int UsrSmp::UnSetStandAloneMode()
{
	QString resp;
	
	switch(modemType)
	{
		case PROFESSIONAL:
				resp = SendCommand("AT+MCA=0");
				if(resp.find("OK") != -1) return 0;
				break;
				
		case NORMAL:
				resp = SendCommand("AT+MCS=0");
				if(resp.find("OK") != -1) return 0;
				break;
				
		default: break;
	}
	
	return 1;
}


/*! \brief Gets memory and message informations from the modem
 *
 *  This function gets information about the memory first.
 *  After that it gets the information about all stored messages
 *
 *  Partly (C) T. Uhlmann from usrmodem.cpp
 */
int UsrSmp::ReadMemoryInfo()
{
	//
	// Get an overview of the stored messages
	//
	QString MemInfoString = SendCommand("AT+MSR=0");
	
	if(MemInfoString.find("OK") == -1)
	{
		kdError(0) << "Couldn' get memory information" << endl;
		return 1;
	}
	
	MemInfoString = MemInfoString.remove("\n");
	MemInfoString = MemInfoString.remove("\r");
	MemInfoString = MemInfoString.remove("OK");
	
	#ifdef DEBUG_USR
		kdDebug(0) << "Memory info string: " << MemInfoString << endl;
	#endif
	
			
	MemInfo.MemSize = MemInfoString.mid(0,3).toInt();
	
	MemInfo.MemUsed = MemInfoString.mid(4,3).toInt();
	
	MemInfo.VoiceMsgs = MemInfoString.mid(8,3).toInt();
	
	MemInfo.UnreleasedVoiceMsgs = MemInfoString.mid(12,3).toInt();
	
	MemInfo.FaxMsgs = MemInfoString.mid(16,3).toInt();
	
	MemInfo.UnreleasedFaxMsgs = MemInfoString.mid(20,3).toInt();
	
	// rest skipped
	
	MemInfo.StoredMsgs = MemInfo.VoiceMsgs + MemInfo.FaxMsgs;
	
	#ifdef DEBUG_USR
		kdDebug(0)  << "\nMemory Size:\t\t" << MemInfo.MemSize
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
		
		QString MsgInfoString = SendCommand(Command);
		
		if(MsgInfoString.find("OK") == -1)
		{
			kdError(0) << "Couldn' get information about message " << message << endl;
			return 2;
		}
		
		MsgInfoString = MsgInfoString.remove("\n");
		MsgInfoString = MsgInfoString.remove("\r");
		MsgInfoString = MsgInfoString.remove("OK");
	
		#ifdef DEBUG_USR
			kdDebug(0) << "Message info string: " << MsgInfoString << endl;
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
		if(MsgInfo->CallerId.isEmpty()) MsgInfo->CallerId = "unknown";
		
		MsgInfo->MemStartPage = MsgInfoString.mid(53,3).toInt();
		MsgInfo->MemAddress = MsgInfoString.mid(57,3).toInt();
		MsgInfo->MemAddress = MsgInfo->MemAddress << 8;
		MsgInfo->MemAddress += MsgInfoString.mid(61,3).toInt();
		
		MsgInfoList.append(MsgInfo);
		
		#ifdef DEBUG_USR
			if(MsgInfo->ClockValid)
			{
				kdDebug(0)  << "\nMessage Number:\t\t" << MsgInfo->MsgNumber
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
				kdDebug(0)  << "\nMessage Number:\t\t" << MsgInfo->MsgNumber
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
struct MemoryInfo UsrSmp::GetMemoryInfo()
{
	return MemInfo;
}


/*! \brief Returns information about all messages
 *
 */
QPtrList<struct MessageInfo> UsrSmp::GetMsgInfo()
{
	return MsgInfoList;
}


/*! \brief Returns information about the specified Message
 *
 */
struct MessageInfo UsrSmp::GetMsgInfo(unsigned int index)
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
void UsrSmp::LoadMessages()
{
	FILE *fd = fopen("/tmp/USRMemory", "wb");
	if(fd == NULL)
	{
		kdError(0) << "Could not open temporary memory file" << endl;
		return;
	}
	
	ReadMemoryToFile(fd);
	
	fclose(fd);
}


/*! \brief This function gets the specified message from the temporary file
 *
 *  Partly (C) T. Uhlmann from usrmodem.cpp
 */
int UsrSmp::GetMessage(int Message)
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
  		#ifdef DEBUG_USR
			kdDebug(0) << "Message found: " << (int)hdr.Index << endl;
		#endif
				
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
int UsrSmp::WriteFaxPage(FILE *msgFile, unsigned char c)
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


/*! \brief Returns the error wich happens in init
 *
 */
int UsrSmp::GetInitError()
{
	return initError;
}





