if HAVE_CPPUNIT

lib_LTLIBRARIES = \
	stateval_loaders_smtest.la

stateval_loaders_smtest_la_SOURCES = \
	TestLoader.cpp              \
	TestLoader.h              \
	Logger.h

stateval_loaders_smtest_la_LIBADD = \
	$(LOG4CXX_LIBS) \
	$(PLUXX_LIBS) \
	$(SIGC_LIBS)

stateval_loaders_smtest_la_CXXFLAGS = \
	-I$(top_srcdir)/include

stateval_loaders_smtest_la_LDFLAGS = \
	-module \
	-avoid-version \
	-shared

AM_CPPFLAGS = \
	$(LOG4CXX_CFLAGS) \
	$(PLUXX_CFLAGS) \
	$(SIGC_CFLAGS)

endif

## File created by the gnome-build tools

