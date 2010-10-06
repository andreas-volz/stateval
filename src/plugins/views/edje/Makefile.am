
lib_LTLIBRARIES = \
	stateval_view_edje.la

stateval_view_edje_la_SOURCES = \
	EcoreDispatcher.cpp    \
	EcoreDispatcher.h    \
	EdjeView.cpp    \
	EdjeView.h    \
	GraphicContext.h

stateval_view_edje_la_LDFLAGS = \
	-module \
	-avoid-version \
	-shared

stateval_view_edje_la_CXXFLAGS = \
	-I \
	../../../../include

stateval_view_edje_la_LIBADD = \
	$(PLUXX_LIBS) \
	$(EFL_LIBS)

AM_CPPFLAGS = \
	$(PLUXX_CFLAGS) \
	$(EFL_CFLAGS)

## File created by the gnome-build tools

