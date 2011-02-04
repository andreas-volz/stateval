IF( COMMAND CMAKE_MINIMUM_REQUIRED )
        CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
ENDIF( COMMAND CMAKE_MINIMUM_REQUIRED )

####################################################################################
# The name of the project
####################################################################################
PROJECT(stateval)

####################################################################################
# Make sure the compiler can find include files from our project.
####################################################################################
INCLUDE_DIRECTORIES (${PROJECT_SOURCE_DIR}/include) 

####################################################################################
# The libs to build to.
####################################################################################
ADD_LIBRARY(stateval STATIC 
    Action.cpp
    ChangeVariableAction.cpp
    CompoundState.cpp
    Condition.cpp
    DecisionState.cpp
    Exceptions.cpp
    FireEventAction.cpp
    HistoryState.cpp
    Loader.cpp
#    LoaderPluginLoader.cpp
#    LoaderPluginLoader.h
    MemoryUtil.h
    searchFile.cpp
    searchFile.h
    SimpleState.cpp
    State.cpp
    Statemachine.cpp
    StateMachineAccessor.cpp
    StateMachineAccessorPImpl.h
    StateMachineThread.cpp
    stringUtil.cpp
    stringUtil.h
    Thread.cpp
    Transition.cpp
    Variable.cpp
    View.cpp
    ViewPluginLoader.cpp
    ViewPluginLoader.h
    ViewState.cpp
)

####################################################################################
# The executeable(s) to build to.
####################################################################################
#ADD_EXECUTABLE(projectExe src/file1.c)

#TARGET_LINK_LIBRARIES(projectExe pthread)

####################################################################################
# Additional Directories to deal with...
####################################################################################
ADD_SUBDIRECTORY(plugins EXCLUDE_FROM_ALL)

# INSTALL(TARGETS projectExe DESTINATION /usr/bin)
