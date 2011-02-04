IF( COMMAND CMAKE_MINIMUM_REQUIRED )
        CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
ENDIF( COMMAND CMAKE_MINIMUM_REQUIRED )

####################################################################################
# The libs to build to.
####################################################################################
ADD_LIBRARY(stateval_loaders_smtest SHARED
    Logger.h
    TestLoader.cpp
    TestLoader.h
)

# INSTALL(TARGETS stateval_loaders_smtest DESTINATION /usr/bin)
