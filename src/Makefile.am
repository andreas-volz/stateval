## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	$(SIGC_CFLAGS) \
	$(PLUXX_CFLAGS) \
	$(LOG4CXX_CFLAGS) \
	$(PTHREAD_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

lib_LTLIBRARIES = \
	libstateval.la

libstateval_la_SOURCES = \
	Action.cpp      \
	CompoundState.cpp      \
	Condition.cpp      \
	DecisionState.cpp      \
	Exceptions.cpp      \
	FireEventAction.cpp      \
	HistoryState.cpp      \
	SimpleState.cpp      \
	State.cpp      \
	Statemachine.cpp      \
	Transition.cpp      \
	Variable.cpp      \
	View.cpp      \
	ViewState.cpp      \
	StateMachineAccessor.cpp      \
	StateMachineThread.cpp      \
	Loader.cpp      \
	stringUtil.cpp      \
	stringUtil.h      \
	Thread.cpp \
	MemoryUtil.h \
	ViewManagerPluginLoader.cpp \
	ViewManagerPluginLoader.h \
	searchFile.cpp \
	searchFile.h \
	ChangeVariableAction.cpp \
	StateMachineAccessorPImpl.h \
	ViewCache.cpp \
	Widget.cpp      \
	ViewManager.cpp

libstateval_la_CXXFLAGS = \
	-I$(top_srcdir)/include

libstateval_la_LIBADD =      \
	$(SIGC_LIBS)      \
	$(PLUXX_LIBS)      \
	$(LOG4CXX_LIBS) \
	$(PTHREAD_LIBS)

SUBDIRS = \
	plugins

