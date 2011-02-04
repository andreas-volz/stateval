IF( COMMAND CMAKE_MINIMUM_REQUIRED )
        CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
ENDIF( COMMAND CMAKE_MINIMUM_REQUIRED )

####################################################################################
# The libs to build to.
####################################################################################
ADD_EXECUTABLE(text_views
    InputThread.cpp
    InputThread.h
    Main.cpp
    Main.h
    searchFile.cpp
    searchFile.h
)

# INSTALL(TARGETS text_views DESTINATION /usr/bin)
