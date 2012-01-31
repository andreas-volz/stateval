if HAVE_EFL

lib_LTLIBRARIES = \
	stateval_viewmanager_edje.la

stateval_viewmanager_edje_la_SOURCES = \
	EcoreDispatcher.cpp    \
	EcoreDispatcher.h    \
	EdjeView.cpp    \
	EdjeView.h    \
	EdjeViewManager.cpp \
	EdjeViewManager.h \
	GUIThread.cpp \
	GUIThread.h \
	EdjeContext.h

stateval_viewmanager_edje_la_LDFLAGS = \
	-module \
	-avoid-version \
	-shared

stateval_viewmanager_edje_la_CXXFLAGS = \
	-I$(top_srcdir)/include

stateval_viewmanager_edje_la_LIBADD = \
	$(PLUXX_LIBS) \
	$(EFL_LIBS) \
	$(SIGC_LIBS)

AM_CPPFLAGS = \
	$(PLUXX_CFLAGS) \
	$(EFL_CFLAGS) \
	$(SIGC_CFLAGS)
	
endif

## File created by the gnome-build tools

