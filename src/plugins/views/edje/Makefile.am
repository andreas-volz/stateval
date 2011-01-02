
lib_LTLIBRARIES = \
	stateval_views_edje.la

stateval_views_edje_la_SOURCES = \
	EcoreDispatcher.cpp    \
	EcoreDispatcher.h    \
	EdjeView.cpp    \
	EdjeView.h    \
	EdjeContext.h

stateval_views_edje_la_LDFLAGS = \
	-module \
	-avoid-version \
	-shared

stateval_views_edje_la_CXXFLAGS = \
	-I \
	../../../../include

stateval_views_edje_la_LIBADD = \
	$(PLUXX_LIBS) \
	$(EFL_LIBS) \
	$(ELEMENTARY_LIBS)

AM_CPPFLAGS = \
	$(PLUXX_CFLAGS) \
	$(EFL_CFLAGS) \
	$(ELEMENTARY_CFLAGS)

## File created by the gnome-build tools

