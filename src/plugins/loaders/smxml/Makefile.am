
lib_LTLIBRARIES = \
	stateval_loaders_smxml.la

stateval_loaders_smxml_la_SOURCES = \
	XMLLoader.cpp            \
	XMLLoader.h            \
	searchFile.cpp            \
	searchFile.h		\
	localUtil.cpp		\
	localUtil.h

stateval_loaders_smxml_la_CXXFLAGS = \
	-I \
	../../../../include

stateval_loaders_smxml_la_LDFLAGS = \
	-module \
	-avoid-version \
	-shared

stateval_loaders_smxml_la_LIBADD = \
	$(PLUXX_LIBS)             \
	$(XML_LIBS)

AM_CPPFLAGS = \
	$(PLUXX_CFLAGS)\
	$(XML_CFLAGS)

## File created by the gnome-build tools

