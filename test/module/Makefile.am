if HAVE_CPPUNIT

bin_PROGRAMS = \
	testThread

testThread_SOURCES = \
	testCommon.h\
	testThread.cpp

testThread_LDADD = \
	../../src/libstateval.la \
	$(SIGC_LIBS) \
	$(CPPUNIT_LIBS) \
	$(PLUXX_LIBS) \
	$(LOG4CXX_LIBS)

testThread_CXXFLAGS = \
	-I../../include

AM_CPPFLAGS = \
	$(SIGC_CFLAGS) \
	$(CPPUNIT_CFLAGS) \
	$(PLUXX_CFLAGS) \
	$(LOG4CXX_CFLAGS)

endif

## File created by the gnome-build tools

