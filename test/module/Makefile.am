if HAVE_CPPUNIT

bin_PROGRAMS = \
	UnitTest

UnitTest_SOURCES = \
	TestMain.cpp \
	TestMain.h \
	VariableTest.cpp 

UnitTest_LDADD = \
	../../src/lib/libstateval.la \
	$(SIGC_LIBS) \
	$(CPPUNIT_LIBS) \
	$(PLUXX_LIBS) \
	$(LOG4CXX_LIBS) \
	-lz

UnitTest_CXXFLAGS = \
	-I../../include

AM_CPPFLAGS = \
	$(SIGC_CFLAGS) \
	$(CPPUNIT_CFLAGS) \
	$(PLUXX_CFLAGS) \
	$(LOG4CXX_CFLAGS)

endif

## File created by the gnome-build tools

