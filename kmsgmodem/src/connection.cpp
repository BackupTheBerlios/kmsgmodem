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

#include "connection.h"

#include <kdebug.h>

#include <stdlib.h>


/*! \brief C'tor
 *
 *  In the C'tor the specified ttyS is initialized.
 *  To be sure that everything is OK in here call ErrorOnInit.
 */
Connection::Connection(QString interface, QString baud) : interface(interface)
{			
	initError = false;
			
	//
	// Select Baudrate, if there is no correct one, use the default
	//
	if(baud == "2400")
  	{
  		baudrate = B2400;
  	}
	else if(baud == "4800")
 	{
  		baudrate = B4800;
  	}
	else if(baud == "9600")
  	{
  		baudrate = B9600;
  	} 
	else if(baud == "19200")
  	{
  		baudrate = B19200;
  	} 
	else if(baud == "38400")
  	{
  		baudrate = B38400;
  	} 
	else if(baud == "57600")
  	{
  		baudrate = B57600;
  	}
	else if(baud == "115200")
  	{
  		baudrate = B115200;
  	}
	else if(baud == "230400")
  	{
  		baudrate = B230400;
  	} 
	else baudrate = DEFAULT_BAUDRATE;
	
	#ifdef DEBUG_CON
		kdDebug(0) << "baudrate: " << baudrate << endl;
	#endif	

	//
	// Open the device and check if it's a tty
	//
	fd = open(interface, O_RDWR | O_NOCTTY); 
	if(fd < 0) 
	{
		kdError(0) << "Could not open " << interface << endl;
		initError = true;
		return;
	}
	
	if(!isatty(fd))
	{
		kdError(0) << interface << " is not a TTY" << endl;
		initError = true;
		return;
	}
	
	#ifdef DEBUG_CON
		kdDebug(0) << this->interface << " open" << endl;
	#endif
	
	//
	// taken from modemconnector.cpp by U. Thulmann (C)
	//
	tcgetattr(fd, &oldState);
	tcgetattr(fd, &newState);

  	newState.c_cflag = baudrate | CRTSCTS | CS8 | CREAD | CLOCAL;
  	newState.c_iflag = INPCK | IGNPAR;
  	newState.c_oflag = OPOST | ONLCR;
  	newState.c_lflag = 0;
  	newState.c_cc[VMIN] = 0;
  	newState.c_cc[VTIME] = 10;
  	cfsetispeed(&newState, baudrate);
  	cfsetospeed(&newState, baudrate);

  	tcflush(fd, TCIOFLUSH);
  	if(tcsetattr(fd,TCSANOW,&newState) < 0)
	{
		kdError(0) << "Could not set new parameters on " << interface << endl;
		initError = true;
		return;
	}
}


/*! \brief D'tor
 *
 */
Connection::~Connection()
{
	if(tcsetattr(fd, TCSANOW, &oldState)  < 0)
	{
		kdError(0) << "Could not set old parameters on " << interface << endl;
	}
	close(fd);
}


/*! \brief This is the main write function.
 *
 *	This function sends the specified signs to the modem.
 */
int Connection::Write(QString command)
{			
	if(write(fd, command, command.length()) <= 0)
	{
		kdError(0) << "Could not write" << endl;
		return 1;
	}
	
	#ifdef DEBUG_CON
		kdDebug(0) << "sent " << command << endl;
	#endif
	
	return 0;
}


/*! \brief This is the main read function.
 *
 *	This function returns a response from the modem.
 */
QString Connection::Read()
{
	char buf[1000];		// We don't know how much we receive in here
	unsigned int rec = 0;
	
	//
	// We read 10 characters from the tty until there is a \n or a \r.
	//
	do
	{
		rec += read(fd, &(buf[rec]), 10);
		if(rec >= 1000) break;
	}
	while((buf[rec-1] != '\r') && (buf[rec-1] != '\n'));
	
	buf[rec] = '\0';	// the modem sends no \0, stupid protocol

	QString response(buf);
	
	#ifdef DEBUG_CON
		kdDebug(0) << "received " << response << endl;
	#endif
	
	return response;
}


/*! \brief Flushes the serial line
 *
 */
void Connection::Flush()
{
	tcflush(fd, TCIOFLUSH);
}


/*! \brief Get any error that may happened in the init process
 *
 */
bool Connection::ErrorOnInit()
{
	return initError;
}


/*! \brief Reads from the tty till the modem stopped sending
 *
 *  (C) T. Uhlmann from usrmodem.cpp
 */
int Connection::SimpleRead()
{
	
	unsigned char c[2] = "";
	c[1] = '\0';

  	if(read(fd, c, 1) < 1)
		return -1;
	else
		return c[0];
}
