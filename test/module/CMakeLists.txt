IF( COMMAND CMAKE_MINIMUM_REQUIRED )
        CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
ENDIF( COMMAND CMAKE_MINIMUM_REQUIRED )

####################################################################################
# The libs to build to.
####################################################################################
ADD_EXECUTABLE(tesmodule
    testCommon.h
    testTemplate.cpp
    testThread.cpp
)

# INSTALL(TARGETS smtest DESTINATION /usr/bin)
