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
#include "config.h"


Config *Config::instance = NULL;

Config::Config()
 : KConfigSkeleton()
{
	setCurrentGroup("General");
	addItemBool("CheckOnStart", CheckOnStart, true);
	addItemBool("SetStandAloneModeOnExit", SetStandAloneModeOnExit, true);
	
	setCurrentGroup("Modem");
	addItemString("Baudrate", Baudrate, "115200");
	addItemString("Port", Port, "/dev/ttyS0");
	
	setCurrentGroup("Messages");
	addItemInt("NoOfVoiceMsgs", NoOfVoiceMsgs, -1);
	addItemInt("NoOfFaxMsgs", NoOfFaxMsgs, -1);
	
	readConfig();
}


Config::~Config()
{
}

Config *Config::Self()
{
	if(instance == NULL)
	{
		instance = new Config();
	}
	
	return instance;
}


