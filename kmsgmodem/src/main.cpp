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
/*
 * Copyright (C) 2004 Alexander Wiedenbruch <wirr@abacho.de>
 */
 
 // TODO list
 // 1. use arts for output
 // 2. implement more gui features, e.g. save fax...
 // 3. continue implement modem features

#include "kmsgmodem.h"
#include <kuniqueapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>

static const char description[] =
    I18N_NOOP("USR Sportster Message Center");

static const char version[] = "0.2";

static KCmdLineOptions options[] =
{
//    { "+[URL]", I18N_NOOP( "Document to open." ), 0 },
    KCmdLineLastOption
};

int main(int argc, char **argv)
{
    KAboutData about("kmsgmodem", I18N_NOOP("KMsgModem"), version, description,
                     KAboutData::License_GPL, "(C) 2004 Alexander Wiedenbruch", 0, 0, "wirr@abacho.de");
    about.addAuthor("Alexander Wiedenbruch", 0, "wirr@abacho.de");
	
	about.addCredit("Torsten Uhlmann", I18N_NOOP("His previous work saved me much time"), "TUhlmann@gmx.de", 0);
	about.addCredit("Jutta Degener, Carsten Bormann", I18N_NOOP("The GSM decompression algos"), 0, "http://kbs.cs.tu-berlin.de/~jutta/toast.html");
    KCmdLineArgs::init(argc, argv, &about);
    KCmdLineArgs::addCmdLineOptions( options );
    KUniqueApplication app;
    KMsgModem *mainWin = 0;

    if (app.isRestored())
    {
        RESTORE(KMsgModem);
    }
    else
    {
        // no session.. just start up normally
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

        /// @todo do something with the command line args here

        mainWin = new KMsgModem();
        app.setMainWidget( mainWin );
		mainWin->setApp(&app);
        mainWin->show();

        args->clear();
    }

    // mainWin has WDestructiveClose flag by default, so it will delete itself.
    return app.exec();
}

