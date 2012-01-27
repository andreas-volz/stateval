IF( COMMAND CMAKE_MINIMUM_REQUIRED )
        CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
ENDIF( COMMAND CMAKE_MINIMUM_REQUIRED )

####################################################################################
# The libs to build to.
####################################################################################
ADD_LIBRARY(stateval_views_text SHARED
    FileReader.cpp
    FileReader.h
    TextView.cpp
    TextView.h
)

# INSTALL(TARGETS stateval_views_text DESTINATION /usr/bin)
