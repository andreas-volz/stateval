
lib_LTLIBRARIES = \
	stateval_views_text.la

stateval_views_text_la_SOURCES = \
	TextView.cpp\
	TextView.h \
	FileReader.cpp \
	FileReader.h

stateval_views_text_la_LDFLAGS = \
	-module \
	-avoid-version \
	-shared

stateval_views_text_la_LIBADD = \
	$(PLUXX_LIBS)

stateval_views_text_la_CXXFLAGS = \
	-I$(top_srcdir)/include

AM_CPPFLAGS = \
	$(PLUXX_CFLAGS)

## File created by the gnome-build tools

