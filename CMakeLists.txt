IF( COMMAND CMAKE_MINIMUM_REQUIRED )
        CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
ENDIF( COMMAND CMAKE_MINIMUM_REQUIRED )

####################################################################################
# The name of the project
####################################################################################
PROJECT(stateval)

####################################################################################
# Target OS specific flags.
####################################################################################
IF(APPLE)
    # Flags for universal build.
#    SET(CMAKE_OSX_ARCHITECTURES ppc;i386)

    # Determine the SDK the source has to build for.
    SET(OSX_SDK 10.5)

    SET(CMAKE_OSX_SYSROOT "/Developer/SDKs/MacOSX${OSX_SDK}.sdk")
    
    ADD_DEFINITIONS(-DDONT_USE_ECORE)
    
ENDIF (APPLE)

####################################################################################
# Make sure the compiler can find include files from our project.
####################################################################################
INCLUDE_DIRECTORIES (${PROJECT_SOURCE_DIR}/include) 

####################################################################################
# The libs to build to.
####################################################################################
ADD_LIBRARY(stateval STATIC 
    src/Action.cpp
    src/CompoundState.cpp
    src/Condition.cpp
    src/DecisionState.cpp
    src/Exceptions.cpp
    src/FileReader.cpp
    src/FireEventAction.cpp
    src/GlobalVariables.cpp
    src/HistoryState.cpp
    src/localUtil.cpp
    src/SimpleState.cpp
    src/SMLoader.cpp
    src/State.cpp
    src/Statemachine.cpp
    src/StateMachineAccess.cpp
    src/StateMachineThread.cpp
    src/stringUtil.cpp
    src/TextView.cpp
    src/Transition.cpp
    src/Variable.cpp
    src/View.cpp
    src/ViewState.cpp
#    src/EcoreDispatcher.cpp
#    src/EdjeView.cpp
#    src/GraphicContext.cpp
    
    src/FileReader.h
    src/localUtil.h
    src/stringUtil.h
)

SET(LIBSIGCPP_DIR "${PROJECT_SOURCE_DIR}/../libsigc++-2.2.8")
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

    ADD_DEPENDENCIES(stateval sigc++)

    INCLUDE_DIRECTORIES (
        ${LIBSIGCPP_DIR}
        ${LIBSIGCPP_DIR}/sigc++ )

ENDIF()


####################################################################################
# The executeable(s) to build to.
####################################################################################
#ADD_EXECUTABLE(projectExe
#    src/file1.c
#    include/file1.h
#)

#TARGET_LINK_LIBRARIES(projectExe pthread)

####################################################################################
# Additional Directories to deal with...
####################################################################################
# ADD_SUBDIRECTORY(lib)

# INSTALL(TARGETS projectExe DESTINATION /usr/bin)
