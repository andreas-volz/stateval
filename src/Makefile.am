## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	$(EFL_CFLAGS) \
	$(SIGC_CFLAGS) \
	$(PLUXX_CFLAGS)

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
	EcoreDispatcher.cpp      \
	EdjeView.cpp      \
	Exceptions.cpp      \
	FireEventAction.cpp      \
	GlobalVariables.cpp      \
	HistoryState.cpp      \
	SimpleState.cpp      \
	State.cpp      \
	Statemachine.cpp      \
	Transition.cpp      \
	Variable.cpp      \
	View.cpp      \
	ViewState.cpp      \
	StateMachineAccess.cpp      \
	StateMachineThread.cpp      \
	Loader.cpp      \
	stringUtil.cpp      \
	stringUtil.h      \
	GraphicContext.cpp      \
	Thread.cpp \
	TextView.cpp \
	localUtil.cpp \
	localUtil.h

libstateval_la_CXXFLAGS = \
	-I \
	../include

libstateval_la_LIBADD =      \
	$(EFL_LIBS)      \
	$(SIGC_LIBS)      \
	$(PLUXX_LIBS)

SUBDIRS = \
	plugins

