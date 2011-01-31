## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	$(EFL_CFLAGS) \
	$(SIGC_CFLAGS) \
	$(PLUXX_CFLAGS) \
	$(LOG4CXX_CFLAGS)

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
	ViewPluginLoader.cpp \
	ViewPluginLoader.h \
	searchFile.cpp \
	searchFile.h \
	ChangeVariableAction.cpp \
	StateMachineAccessorPImpl.h

libstateval_la_CXXFLAGS = \
	-I \
	../include

libstateval_la_LIBADD =      \
	$(EFL_LIBS)      \
	$(SIGC_LIBS)      \
	$(PLUXX_LIBS)      \
	$(LOG4CXX_LIBS)

SUBDIRS = \
	plugins

