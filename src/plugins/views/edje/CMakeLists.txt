IF( COMMAND CMAKE_MINIMUM_REQUIRED )
        CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
ENDIF( COMMAND CMAKE_MINIMUM_REQUIRED )

####################################################################################
# The libs to build to.
####################################################################################
ADD_LIBRARY(edje SHARED
    EcoreDispatcher.cpp
    EcoreDispatcher.h
    EdjeContext.cpp
    EdjeContext.h
    EdjeView.cpp
    EdjeView.h
    Logger.h
)

# INSTALL(TARGETS edje DESTINATION /usr/bin)
