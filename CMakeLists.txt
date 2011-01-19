IF( COMMAND CMAKE_MINIMUM_REQUIRED )
        CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
ENDIF( COMMAND CMAKE_MINIMUM_REQUIRED )

####################################################################################
# The name of the project
####################################################################################
PROJECT(stateval)

# The base path for libsigc++
SET(LIBSIGCPP_DIR "${PROJECT_SOURCE_DIR}/../libsigc++-2.2.8")

# The base path for pluxx
SET(LIBPLUXX_DIR "${PROJECT_SOURCE_DIR}/../../pluxx")


####################################################################################
# Target OS specific flags.
####################################################################################
IF(APPLE)
    ADD_DEFINITIONS(-DDONT_USE_ECORE)
    FIND_LIBRARY(CPPUNIT cppunit)
    MARK_AS_ADVANCED (CPPUNIT)
    SET(EXTRA_LIBS ${CPPUNIT})
ELSE()
    SET(EXTRA_LIBS cppunit)
ENDIF (APPLE)

SET(EXTRA_LIBS pthread ${EXTRA_LIBS})

####################################################################################
# Make sure the compiler can find include files from our project.
####################################################################################
INCLUDE_DIRECTORIES (${PROJECT_SOURCE_DIR}/include) 

####################################################################################
# The libs to build to.
####################################################################################
ADD_LIBRARY(stateval STATIC 
    src/Action.cpp
    src/ChangeVariableAction.cpp
    src/CompoundState.cpp
    src/Condition.cpp
    src/DecisionState.cpp
    src/Exceptions.cpp
    src/FireEventAction.cpp
    src/GlobalVariables.cpp
    src/HistoryState.cpp
    src/Loader.cpp
#    src/LoaderPluginLoader.cpp
    src/plugins/loaders/smxml/searchFile.cpp
    src/plugins/loaders/smxml/stringUtil.cpp
#    src/plugins/loaders/smxml/XMLLoader.cpp
#    src/plugins/views/edje/EcoreDispatcher.cpp
#    src/plugins/views/edje/EdjeContext.cpp
#    src/plugins/views/edje/EdjeView.cpp
    src/plugins/views/text/FileReader.cpp
    src/plugins/views/text/TextView.cpp
    src/searchFile.cpp
    src/SimpleState.cpp
    src/State.cpp
    src/Statemachine.cpp
    src/StateMachineAccess.cpp
    src/StateMachineThread.cpp
    src/stringUtil.cpp
    src/Thread.cpp
    src/Transition.cpp
    src/Variable.cpp
    src/View.cpp
    src/ViewPluginLoader.cpp
    src/ViewState.cpp

# include
    include/stateval/Action.h
    include/stateval/ChangeVariableAction.h
    include/stateval/CompoundState.h
    include/stateval/Condition.h
    include/stateval/Context.h
    include/stateval/DecisionState.h
    include/stateval/Exceptions.h
    include/stateval/FireEventAction.h
    include/stateval/GlobalVariables.h
    include/stateval/HistoryState.h
    include/stateval/Loader.h
    include/stateval/SimpleState.h
    include/stateval/State.h
    include/stateval/StateMachine.h
    include/stateval/StateMachineAccess.h
    include/stateval/StateMachineThread.h
    include/stateval/stateval.h
    include/stateval/Thread.h
    include/stateval/Transition.h
    include/stateval/Variable.h
    include/stateval/View.h
    include/stateval/ViewState.h
)

# Build sigc++ if its sourcecode exists on a certain place (here '../libsigc++-2.2.8')
IF (EXISTS ${LIBSIGCPP_DIR}/)
    
    ADD_LIBRARY(sigc++ STATIC
        ${LIBSIGCPP_DIR}/sigc++/adaptors/lambda/lambda.cc
        ${LIBSIGCPP_DIR}/sigc++/connection.cc
        ${LIBSIGCPP_DIR}/sigc++/functors/slot.cc
        ${LIBSIGCPP_DIR}/sigc++/functors/slot_base.cc
        ${LIBSIGCPP_DIR}/sigc++/signal.cc
        ${LIBSIGCPP_DIR}/sigc++/signal_base.cc
        ${LIBSIGCPP_DIR}/sigc++/trackable.cc
    )

    ADD_DEPENDENCIES(stateval sigc++ pluxx)

    INCLUDE_DIRECTORIES (
        ${LIBSIGCPP_DIR}
        ${LIBSIGCPP_DIR}/sigc++ )

ENDIF()

# build pluxx if there is a ${LIBPLUXX_DIR}/CMakeLists.txt
IF (EXISTS ${LIBPLUXX_DIR}/CMakeLists.txt)
    ADD_SUBDIRECTORY(${LIBPLUXX_DIR} pluxx)
ENDIF()

INCLUDE_DIRECTORIES (${LIBPLUXX_DIR}/include)

####################################################################################
# The executeable(s) to build to.
####################################################################################
ADD_EXECUTABLE(testThread EXCLUDE_FROM_ALL
    test/module/testThread.cpp
)

TARGET_LINK_LIBRARIES(testThread z sigc++ stateval pluxx ${EXTRA_LIBS})

####################################################################################
# Additional Directories to deal with...
####################################################################################
# INSTALL(TARGETS projectExe DESTINATION /usr/bin)
