# set the include path for X, qt and KDE
INCLUDES = $(all_includes)

# these are the headers for your project
noinst_HEADERS = kmsgmodem.h connection.h simplemodem.h settings.h config.h generalsettings.h modemsettings.h usrsmpthread.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

rcdir = $(kde_datadir)/kmsgmodem
rc_DATA = kmsgmodemui.rc

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/kmsgmodem.pot

KDE_ICON = kmsgmodem

#########################################################################
# APPLICATION SECTION
#########################################################################
# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = kmsgmodem

# the application source, library search path, and link libraries
kmsgmodem_SOURCES = main.cpp kmsgmodem.cpp kmsgmodemwidget.cpp kmsgmodemwidgetbase.ui connection.cpp simplemodem.cpp config.cpp GeneralWidget.ui generalsettings.cpp Modem.ui modemsettings.cpp usrsmpthread.cpp
kmsgmodem_LDFLAGS = $(KDE_RPATH) $(all_libraries)
kmsgmodem_LDADD = -lartskde $(LIB_KDEUI)

# this is where the desktop file will go 
shelldesktopdir   = $(kde_appsdir)/Utilities
shelldesktop_DATA = kmsgmodem.desktop

# this is where the shell's XML-GUI resource file goes
shellrcdir   = $(kde_datadir)/kmsgmodem
