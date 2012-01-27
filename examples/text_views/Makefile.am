
bin_PROGRAMS = \
	text_views

text_views_SOURCES = \
	Main.cpp    \
	Main.h    \
	searchFile.cpp    \
	searchFile.h

text_views_CXXFLAGS = \
	-I$(top_srcdir)/include \
	$(SIGC_CFLAGS) \
	$(EFL_CFLAGS) \
	$(PTHREAD_CFLAGS)

text_views_LDADD = \
	$(top_srcdir)/src/libstateval.la \
	$(SIGC_LIBS) \
	$(EFL_LIBS) \
	$(PLUXX_LIBS) \
	$(LOG4CXX_LIBS) \
	$(PTHREAD_LIBS)

AM_CPPFLAGS = \
	$(PLUXX_CFLAGS)\
	$(LOG4CXX_CFLAGS)

## File created by the gnome-build tools

