IF( COMMAND CMAKE_MINIMUM_REQUIRED )
        CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
ENDIF( COMMAND CMAKE_MINIMUM_REQUIRED )

####################################################################################
# The libs to build to.
####################################################################################
ADD_LIBRARY(text SHARED
    FileReader.cpp
    FileReader.h
    TextView.cpp
    TextView.h
)

# INSTALL(TARGETS text DESTINATION /usr/bin)
