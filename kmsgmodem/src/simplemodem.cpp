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

#include <time.h> 

#include "simplemodem.h"

#include <kdebug.h>

/*! \brief C'tor
 *
 *  Mainly for tty init
 */
SimpleModem::SimpleModem(QString interface, QString baud)
{
	port = new Connection(interface, baud);
	
	if(port->ErrorOnInit())
	{
		delete port;
		port = NULL;
	}
}


/*! \brief D'tor
 *
 */
SimpleModem::~SimpleModem()
{
	delete port;
}


/*! \brief This function sends a command to the modem
 *
 *  Call this function to transmit a command to the
 *  modem.
 *  As result the response of the modem is returned.
 *  The function automatically adds a \r at the end of
 *  the command.
 */
QString SimpleModem::SendCommand(QString command)
{
	if(port == NULL)
	{
		QString error;
		return error;
	}
	
	command.append("\r");

	port->Flush();		// Clear line

	if(port->Write(command) > 0)
	{
		QString error;
		return error;
	}
	
	QString resp;
	
	//
	// We read from the tty until we receive the status
	// from the command.
	//
	do
	{
		usleep(200000);		// wait for response
		resp += port->Read();
	}
	while((!ResponseOK(resp)));
		
	return resp;
}


/*! \brief This class checks the string for a status-word from the modem
 *
 */
bool SimpleModem::ResponseOK(QString command)
{
	if(	(command.find("OK\r") >= 0) ||
		(command.find("ERROR\r") >= 0) ||
		(command.find("RING\r") >= 0) ||
		(command.find("NO CARRIER\r") >= 0) ||
		(command.find("CONNECT") >= 0) ||  // possible baud speed after response
		(command.find("NO DIALTONE\r") >= 0) ||
		(command.find("BUSY\r") >= 0) ||
		(command.find("NO ANSWER\r") >= 0) )
	{
		return true;
	}
	else
		return false;
}


/*! \brief This function reads from the modem and write it to the specified file
 *
 *  Partly (C) T. Uhlmann from usrmodem.cpp
 */
void SimpleModem::ReadMemoryToFile(FILE *fd)
{
	if(port->Write("AT+MTM\r") > 0)
	{
		return;
	}
	
	usleep(200000);

	unsigned int count = 1;
	for(;;)
	{
		int rc = port->SimpleRead();
		if(rc < 0) return;
		else
		{
			//kdDebug(0) << "fputc " << (unsigned char)rc << endl;
			if(count > 2) fputc((unsigned char) rc, fd);
			count++;
			if(count >= MODEM_USR_PAGE + 2) count = 1;
		}
	}
}

/*! \brief This function returns a pointer to the connection
 *
 */
Connection* SimpleModem::GetConnection()
{
	return port;
}

