
bin_PROGRAMS = \
	text_views

text_views_SOURCES = \
	Main.cpp    \
	Main.h    \
	InputThread.cpp    \
	InputThread.h    \
	searchFile.cpp    \
	searchFile.h

text_views_CXXFLAGS = \
	-I../../include \
	$(SIGC_CFLAGS) \
	$(EFL_CFLAGS)

text_views_LDADD = \
	../../src/libstateval.la \
	$(SIGC_LIBS) \
	$(EFL_LIBS) \
	$(PLUXX_LIBS)

AM_CPPFLAGS = \
	$(PLUXX_CFLAGS)

## File created by the gnome-build tools

