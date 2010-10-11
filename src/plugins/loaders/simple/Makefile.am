
lib_LTLIBRARIES = \
	stateval_loaders_simple.la

stateval_loaders_simple_la_SOURCES = \
	SimpleLoader.cpp\
	SimpleLoader.h \
	FileReader.cpp \
	FileReader.h \
	localUtil.cpp \
	localUtil.h \
	stringUtil.cpp \
	stringUtil.h \
	searchFile.cpp \
	searchFile.h

stateval_loaders_simple_la_CXXFLAGS = \
	-I \
	../../../../include

stateval_loaders_simple_la_LIBADD = \
	$(EFL_LIBS) \
	$(PLUXX_LIBS)

stateval_loaders_simple_la_LDFLAGS = \
	-module \
	-avoid-version \
	-shared

AM_CPPFLAGS = \
	$(EFL_CFLAGS)\
	$(PLUXX_CFLAGS)

## File created by the gnome-build tools

