# set the include path for X, qt and KDE
INCLUDES = $(all_includes)

# these are the headers for your project
noinst_HEADERS = kmsgmodem.h filebase.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

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
kmsgmodem_SOURCES = main.cpp kmsgmodem.cpp connection.cpp generalsettings.cpp \
	kmsgmodemwidget.cpp modemsettings.cpp modemsettingsdialog.cpp mymodemsettingsdialog.cpp \
	simplemodem.cpp usrsmpthread.cpp GeneralWidget.ui kmsgmodemwidgetbase.ui \
	ModemSettings.ui Modem.ui settings.cpp filebase.cpp
kmsgmodem_LDFLAGS = $(KDE_RPATH) $(all_libraries)
kmsgmodem_LDADD = -lartskde $(LIB_KDEUI)

# this is where the desktop file will go 
shelldesktopdir   = $(kde_appsdir)/Utilities
shelldesktop_DATA = kmsgmodem.desktop

# this is where the shell's XML-GUI resource file goes
shellrcdir   = $(kde_datadir)/kmsgmodem
shellrc_DATA = kmsgmodemui.rc

