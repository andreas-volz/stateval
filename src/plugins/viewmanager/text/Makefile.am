
lib_LTLIBRARIES = \
	stateval_viewmanager_text.la

stateval_viewmanager_text_la_SOURCES = \
	TextView.cpp\
	TextView.h \
	FileReader.cpp \
	FileReader.h \
	TextViewManager.cpp \
	TextViewManager.h \
	InputThread.cpp    \
	InputThread.h

stateval_viewmanager_text_la_LDFLAGS = \
	-module \
	-avoid-version \
	-shared

stateval_viewmanager_text_la_LIBADD = \
	$(PLUXX_LIBS) \
	$(SIGC_LIBS) 

stateval_viewmanager_text_la_CXXFLAGS = \
	-I$(top_srcdir)/include

AM_CPPFLAGS = \
	$(PLUXX_CFLAGS) \
	$(SIGC_CFLAGS) 

## File created by the gnome-build tools

