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
#ifndef USRSMP_H
#define USRSMP_H

#include <qstring.h>
#include <qptrlist.h>

#include "simplemodem.h"

#define NORMAL			1
#define PROFESSIONAL	2
#define UNSPECIFIED		3

#define NONE			4
#define RESET_ERROR		5
#define UNSUP			6

// kfax -normal /tmp/fax-*
// /home/wirr/gsm-1.0-pl10/bin/tcat -d "/tmp/fax-1" > /dev/audio

/*
struct MemoryInfo
{
	int MemSize;
	int MemUsed;
	int VoiceMsgs;
	int UnreleasedVoiceMsgs;
	int FaxMsgs;
	int UnreleasedFaxMsgs;
	int StoredMsgs;		// only for convenience
};

struct MessageInfo
{
	int MsgNumber;
	int MsgType;
	int MsgSize;
	int MsgAttribute;
	int	MsgStatus;
	bool ClockValid;
	int ReceiveDay;
	int ReceiveHour;
	int ReceiveMinute;
	QString CallerId;
	int MemStartPage;
	int MemAddress;
};

struct Header {
  unsigned char Index;
  unsigned char Type;
  unsigned char Size;
  unsigned char MesgAttributes;
  unsigned char ReceptStatus;
  unsigned char Days;
  unsigned char Hours;
  unsigned char Minutes;
  unsigned char Sender[20];
  unsigned char PrevPage;
  unsigned char PrevAddr1;
  unsigned char PrevAddr2;
  unsigned char NextPage;
  unsigned char NextAddr1;
  unsigned char NextAddr2;
}; /* 34 byte 
*/

/**
@author Alexander Wiedenbruch
*/
class UsrSmp : private SimpleModem
{
public:
    UsrSmp(QString interface = "/dev/ttyS0", QString baud = "115200");

    ~UsrSmp();
	
	void SetEchoOff();
	
	int GetModemType();
	
	int SetStandAloneMode();
	
	int UnSetStandAloneMode();
	
	int ReadMemoryInfo();
	
	QPtrList<struct MessageInfo> GetMsgInfo();
	
	struct MessageInfo UsrSmp::GetMsgInfo(unsigned int index);
			
	struct MemoryInfo GetMemoryInfo();
	
	void LoadMessages();
	
	int GetMessage(int Message);
	
	int GetInitError();
	
private:
	
	int DetectModemType();
	
	int WriteFaxPage(FILE *msgFile, unsigned char c);
	
	int modemType;
	
	int WasDLE; //????
	
	struct MemoryInfo MemInfo;
	
	QPtrList<struct MessageInfo> MsgInfoList;
	
	int initError;
};

#endif
