if HAVE_EFL

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
	-I$(top_srcdir)/include \
	$(EFL_CFLAGS) \
	$(SIGC_CFLAGS) \
	-I$(top_srcdir)/src

edje_console_LDADD = \
	$(top_srcdir)/src/libstateval.la \
	$(EFL_LIBS) \
	$(SIGC_LIBS) \
	$(PLUXX_LIBS) \
	$(LOG4CXX_LIBS)

AM_CPPFLAGS = \
	$(PLUXX_CFLAGS)\
	$(LOG4CXX_CFLAGS)

endif

## File created by the gnome-build tools

