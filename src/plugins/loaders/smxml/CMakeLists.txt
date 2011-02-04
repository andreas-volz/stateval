IF( COMMAND CMAKE_MINIMUM_REQUIRED )
        CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
ENDIF( COMMAND CMAKE_MINIMUM_REQUIRED )

####################################################################################
# The libs to build to.
####################################################################################
ADD_LIBRARY(smxml SHARED
    Logger.h
    MemoryUtil.h
    searchFile.cpp
    searchFile.h
    stringUtil.cpp
    stringUtil.h
    XMLLoader.cpp
    XMLLoader.h
)

# INSTALL(TARGETS smxml DESTINATION /usr/bin)
