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
#ifndef CONNECTION_H
#define CONNECTION_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

#include <qstring.h>

//#define DEBUG_CON
#define DEFAULT_BAUDRATE 	B19200

/*!
 *  This class implements low level code for the serial line.
 *
 *	@todo use QextSerailPort in next version
 */
class Connection
{
public:
    Connection(QString interface = "/dev/ttyS0", QString baud = "19200");

    ~Connection();
	
	int Port();
	
	int Write(QString command);
	
	QString Read();
	
	void Flush();
	
	bool ErrorOnInit();

	int SimpleRead();
			
private:
	
	QString interface;
	
	int fd;
	struct termios oldState, newState;
	
	tcflag_t baudrate;
	
	bool initError;

};

#endif
