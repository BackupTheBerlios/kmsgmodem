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
#include <kstandarddirs.h>
#include <kdebug.h>

#include "filebase.h"

FileBase *FileBase::instance = NULL;

FileBase::FileBase()
{
	MemoryFilename = locateLocal("data", "kmsgmodem/modem/memory");
	
	tmp = new KTempDir();
	tmp->setAutoDelete(true);
	
	MsgDirName = tmp->name();
}


FileBase::~FileBase()
{
	delete tmp;
}


FileBase *FileBase::Self()
{
	if(instance == NULL)
	{
		instance = new FileBase();
	}
	
	return instance;
}

