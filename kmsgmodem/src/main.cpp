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

#include "kmsgmodem.h"
#include <kuniqueapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>

static const char description[] =
    I18N_NOOP("USR Sportster Message Center");

static const char version[] = "0.3";

static KCmdLineOptions options[] =
{
//    { "+[URL]", I18N_NOOP( "Document to open." ), 0 },
    KCmdLineLastOption
};

int main(int argc, char **argv)
{
    KAboutData about("kmsgmodem", I18N_NOOP("KMsgModem"), version, description,
                     KAboutData::License_GPL, "(C) 2004 Alexander Wiedenbruch", 0, 0, "wirr@users.berlios.de ");
    about.addAuthor("Alexander Wiedenbruch", 0, "wirr@users.berlios.de ");
	
	about.addCredit("Torsten Uhlmann", I18N_NOOP("His previous work saved me much time"), 0, 0);
	about.addCredit("Chris Bagwell", I18N_NOOP("Thanks for SoX"), 0, "http://sox.sourceforge.net/");
    KCmdLineArgs::init(argc, argv, &about);
    KCmdLineArgs::addCmdLineOptions( options );
    KUniqueApplication app;
    KMsgModem *mainWin = 0;

    if (app.isRestored())
    {
        RESTORE(KMsgModem(&app));
    }
    else
    {
        // no session.. just start up normally
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

        /// @todo do something with the command line args here

        mainWin = new KMsgModem(&app);
        app.setMainWidget( mainWin );
		
        mainWin->show();
		mainWin->Startup();
		
        args->clear();
    }

    // mainWin has WDestructiveClose flag by default, so it will delete itself.
    return app.exec();
}

