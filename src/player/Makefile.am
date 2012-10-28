
stateval_player_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src/lib \
	$(LOG4CXX_CFLAGS) \
	$(SIGC_CFLAGS) \
	$(PTHREAD_CFLAGS)
## Process this file with automake to produce Makefile.in


bin_PROGRAMS =  \
	stateval_player

stateval_player_SOURCES =  \
	Main.cpp \
	Main.h \
	searchFile.cpp \
	searchFile.h

stateval_player_LDADD = \
	$(PTHREAD_LIBS) \
	$(top_builddir)/src/lib/libstateval.la \
	$(LOG4CXX_LIBS) \
	$(SIGC_LIBS) 

stateval_player_LDFLAGS = \
	-lpthread
