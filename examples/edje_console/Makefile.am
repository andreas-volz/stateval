
bin_PROGRAMS = \
	edje_console

edje_console_SOURCES = \
	InputThread.cpp\
	InputThread.h \
	searchFile.cpp \
	searchFile.h \
	Main.h \
	Main.cpp

edje_console_LDADD = \
	$(GLIB_LIBS) \
	$(EFL_LIBS) \
	../../src/libstateval.la

AM_CPPFLAGS = \
	$(GLIB_CFLAGS) \
	$(EFL_CFLAGS) \
	-I../../include

## File created by the gnome-build tools

