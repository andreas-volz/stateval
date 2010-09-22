
bin_PROGRAMS = \
	text_views

text_views_SOURCES = \
	Main.cpp    \
	Main.h    \
	InputThread.cpp    \
	InputThread.h    \
	searchFile.cpp    \
	searchFile.h

text_views_LDADD = \
	$(GLIB_LIBS) \
	../../src/libstateval.la \
	$(EFL_LIBS)

AM_CPPFLAGS = \
	$(GLIB_CFLAGS) \
	-I../../include \
	$(EFL_CFLAGS)

## File created by the gnome-build tools

