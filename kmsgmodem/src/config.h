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
#ifndef CONFIG_H
#define CONFIG_H

#include <kconfigskeleton.h>

#include <qstring.h>

#include <time.h>

/**
@author Alexander Wiedenbruch
*/
class Config : public KConfigSkeleton
{
protected:
    Config();

public:
    ~Config();
	
	static Config *Self();
	
	bool SetStandAloneModeOnExit;
	bool CheckOnStart;
	
	QString Baudrate;
	QString Port;
	
	int NoOfFaxMsgs;
	int NoOfVoiceMsgs;
	
	time_t ResetTime;

private:	
	static Config *instance;

};

#endif
