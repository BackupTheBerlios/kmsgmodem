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
/*
 * Copyright (C) 2004 Alexander Wiedenbruch <wirr@users.berlios.de>
 */


#include "kmsgmodemwidget.h"

#include <kdebug.h>

#include <klistview.h>

KMsgModemWidget::KMsgModemWidget(QWidget* parent, const char* name, WFlags fl)
        : KMsgModemWidgetBase(parent,name,fl)
{
	connect(this, SIGNAL(FaxMsgClicked(int)), parent, SLOT(ShowFax(int)));
	connect(this, SIGNAL(VoiceMsgClicked(int)), parent, SLOT(PlayVoice(int)));
}

KMsgModemWidget::~KMsgModemWidget()
{
	//delete test;
}

/*$SPECIALIZATION$*/
void KMsgModemWidget::MessageClicked(QListViewItem *item)
{
	if(item == NULL) return;
	
	if(item->text(1) == "Fax")
	{
		emit(FaxMsgClicked(item->text(0).toInt()));
	}
	else
	{
		emit(VoiceMsgClicked(item->text(0).toInt()));
	}
}



#include "kmsgmodemwidget.moc"

