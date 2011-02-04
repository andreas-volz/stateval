IF( COMMAND CMAKE_MINIMUM_REQUIRED )
        CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
ENDIF( COMMAND CMAKE_MINIMUM_REQUIRED )

####################################################################################
# The libs to build to.
####################################################################################
ADD_EXECUTABLE(testmodule
    testCommon.h
    testThread.cpp
)

TARGET_LINK_LIBRARIES(testmodule ${STATEVAL_LIBS})

# INSTALL(TARGETS smtest DESTINATION /usr/bin)
