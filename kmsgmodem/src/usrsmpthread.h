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
#ifndef USRSMPTHREAD_H
#define USRSMPTHREAD_H

#include <qthread.h>
#include <qstring.h>
#include <qptrlist.h>

#include "simplemodem.h"

#define NORMAL			1
#define PROFESSIONAL	2
#define UNSPECIFIED		3

#define NONE			4
#define RESET_ERROR		5
#define UNSUP			6
#define INIT_ERROR		7

#define DEF				8
#define	INIT			9
#define READMEMORY		10
#define LOADMESSAGES	11
#define PLAYVOICE		12
#define ERASEMESSAGES	13

//#define DEBUG_USR

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
}; /* 34 byte */

/**
@author Alexander Wiedenbruch
*/
class UsrSmpThread : public QThread
{
	
public:
    UsrSmpThread(QString interface, QString baud);

    ~UsrSmpThread();
	
	//
	// Thread functions
	//
	void InitModem();
	
	void ReadMemoryInfo();
	
	void run();
	
	void LoadMessages();
	
	void PlayVoiceFile();
	
	void ClearMemory();
	
	//
	// Normal used functions
	//
	int GetInitError();
	
	struct MemoryInfo GetMemoryInfo();
	
	QPtrList<struct MessageInfo> GetMsgInfo();
			
	struct MessageInfo GetMsgInfo(unsigned int index);
	
	int WriteFaxPage(FILE *msgFile, unsigned char c);
	
	int GetMessage(int Message);
	
	int UnSetStandAloneMode();
	
	int SetStandAloneMode();
	
	int GetModemType();
	
private:
	
	//
	// Thread functions
	//
	int InitModemThread();	
		
	int ReadMemoryInfoThread();
	
	void LoadMessagesThread();
	
	void PlayVoiceFileThread();
	
	int ClearMemoryPrivate();
	

	int DetectModemType();
		
	int WasDLE;
	
	int initError;
	
	int modemType;

	int funcToRun;
	
	SimpleModem *simplemodem;
	
	struct MemoryInfo MemInfo;
	
	QPtrList<struct MessageInfo> MsgInfoList;
};

#endif
