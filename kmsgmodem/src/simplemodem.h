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
#ifndef SIMPLEMODEM_H
#define SIMPLEMODEM_H

#include "connection.h"

#include <stdio.h>

#define MODEM_USR_PAGE	32768

#define MODEM_ETX		0x03
#define MODEM_DLE		0x10
#define MODEM_SUB		0x1A

#define MODEM_NEW_PAGE 	100


/*!
 *  This class implements all necessery function to communicate
 *  on a high-level basis with the modem.
 *
 */
class SimpleModem
{

public:
    SimpleModem(QString interface = "/dev/ttyS0", QString baud = "19200");

    ~SimpleModem();
	
	QString SendCommand(QString command);
	
	void ReadMemoryToFile(FILE *fd);
	
	class Connection* GetConnection();
	
private:
	
	Connection *port;
	
	bool ResponseOK(QString command);

};

#endif
