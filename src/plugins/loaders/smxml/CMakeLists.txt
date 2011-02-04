IF( COMMAND CMAKE_MINIMUM_REQUIRED )
        CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
ENDIF( COMMAND CMAKE_MINIMUM_REQUIRED )

####################################################################################
# The name of the project
####################################################################################
PROJECT(XMLLoader)

####################################################################################
# Make sure the compiler can find include files from our project.
####################################################################################
INCLUDE_DIRECTORIES (
    /usr/include/libxml++-2.6
    /usr/include/glibmm-2.4
) 

####################################################################################
# The libs to build to.
####################################################################################
ADD_LIBRARY(XMLLoader SHARED 
	XMLLoader.cpp
	XMLLoader.h
	searchFile.cpp
	searchFile.h
	MemoryUtil.h
	stringUtil.cpp
	stringUtil.h 
	Logger.h
	)

#TARGET_LINK_LIBRARIES(XMLLoader pthread)

####################################################################################
# Additional Directories to deal with...
####################################################################################
# INSTALL(TARGETS XMLLoader DESTINATION /usr/lib)
