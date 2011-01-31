if HAVE_XML

lib_LTLIBRARIES = \
	stateval_loaders_smxml.la

stateval_loaders_smxml_la_SOURCES = \
	XMLLoader.cpp            \
	XMLLoader.h            \
	searchFile.cpp            \
	searchFile.h		\
	MemoryUtil.h            \
	stringUtil.cpp            \
	stringUtil.h            \
	Logger.h

stateval_loaders_smxml_la_CXXFLAGS = \
	-I \
	../../../../include

stateval_loaders_smxml_la_LDFLAGS = \
	-module \
	-avoid-version \
	-shared

stateval_loaders_smxml_la_LIBADD =             \
	$(PLUXX_LIBS)             \
	$(XML_LIBS)             \
	$(LOG4CXX_LIBS)

AM_CPPFLAGS = \
	$(PLUXX_CFLAGS) \
	$(XML_CFLAGS) \
	$(LOG4CXX_CFLAGS)
	
endif

## File created by the gnome-build tools

