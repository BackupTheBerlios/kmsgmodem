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

#ifndef _KMSGMODEM_H_
#define _KMSGMODEM_H_

#ifdef HAVE_CONFIG_H
#include <../config.h>
#endif

#include <kmainwindow.h>
#include <kaction.h>
#include <kstatusbar.h>
#include <kprocess.h>
#include <klocale.h>
#include <kuniqueapplication.h>
#include <kcmdlineargs.h>
#include <arts/kartsdispatcher.h>
#include <arts/kartsserver.h>
#include <arts/kplayobject.h>
#include <arts/kplayobjectfactory.h>

#include "kmsgmodem.h"
#include "kmsgmodemwidget.h"
#include "settings.h"
#include "usrsmpthread.h"
#include "filebase.h"


class UsrSmpThread;
class Config;

/**
 * @short Application Main Window
 * @author Alexander Wiedenbruch <wirr@users.berlios.de>
 * @version 0.1
 */
class KMsgModem : public KMainWindow
{
    Q_OBJECT
public:
    /**
     * Default Constructor
     */
	
    KMsgModem(KUniqueApplication *app);

    /**
     * Default Destructor
     */
    virtual ~KMsgModem();
	
	void Startup();
	
	static void HanldeArgs(KCmdLineArgs *args);
	
private:
	void NewMessage();
	
	QString CalcDate(int Day, int Hour, int Minute);
	
	void EnableModemActions();
	
	void DisableModemActions();
			
	KMsgModemWidget *mainWidget;
	
	UsrSmpThread *modem;
	
	KStatusBar *statusbar;
	
	KAction *standaloneToogle;
	KAction *stop;
	KAction *reload;
	KAction *clearmemory;
	KAction *saveas;
	KAction *preferences;
	KAction *modemPreferences;
	
	bool standaloneModeActive;
	
	KUniqueApplication *application;
	
	KArtsDispatcher dispatcher;
	KArtsServer server;
	KDE::PlayObject *playobj;
	
	enum { VOICE, FAX };
	unsigned int selectedMessage;
	unsigned int selectedMessageType;

private slots:	
	
	void LoadMessages();
	
	void ShowFax(int Fax);
	
	void PlayVoice(int Voice);
	
	void showSettings();
	
	void ToogleStandaloneMode();
	
	void SetStatusbarText(QString text = i18n("Idle"), int item = 0, bool WithTimeout = false);
	
	void StopPlayingVoice();
	
	void ClearMemory();
	
	void SaveFile();
	
	bool queryClose();
	
	void QuitApp();
	
	void showModemSettings();
};

#endif // _KMSGMODEM_H_
