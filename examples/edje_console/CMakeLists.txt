IF( COMMAND CMAKE_MINIMUM_REQUIRED )
        CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
ENDIF( COMMAND CMAKE_MINIMUM_REQUIRED )

####################################################################################
# The libs to build to.
####################################################################################
ADD_EXECUTABLE(edje_console
    InputThread.cpp
    InputThread.h
    Main.cpp
    Main.h
    searchFile.cpp
    searchFile.h
)

# INSTALL(TARGETS edje_console DESTINATION /usr/bin)
