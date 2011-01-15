
bin_PROGRAMS = \
	edje_console

edje_console_SOURCES = \
	InputThread.cpp\
	InputThread.h \
	searchFile.cpp \
	searchFile.h \
	Main.h \
	Main.cpp

edje_console_CXXFLAGS = \
	-I../../include \
	$(EFL_CFLAGS) \
	$(SIGC_CFLAGS) \
	-I../../src

edje_console_LDADD = \
	../../src/libstateval.la \
	$(EFL_LIBS) \
	$(SIGC_LIBS) \
	$(PLUXX_LIBS) \
	$(LOG4CXX_LIBS)

AM_CPPFLAGS = \
	$(PLUXX_CFLAGS)\
	$(LOG4CXX_CFLAGS)

## File created by the gnome-build tools

