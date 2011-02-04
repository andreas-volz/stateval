IF( COMMAND CMAKE_MINIMUM_REQUIRED )
        CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
ENDIF( COMMAND CMAKE_MINIMUM_REQUIRED )

####################################################################################
# The name of the project
####################################################################################
PROJECT(Project)

####################################################################################
# Make sure the compiler can find include files from our project.
####################################################################################
INCLUDE_DIRECTORIES (${PROJECT_SOURCE_DIR}/include) 

####################################################################################
# The libs to build to.
####################################################################################
# ADD_LIBRARY(util SHARED src/file1.c)

####################################################################################
# The executeable(s) to build to.
####################################################################################
#ADD_EXECUTABLE(projectExe src/file1.c)

#TARGET_LINK_LIBRARIES(projectExe pthread)

####################################################################################
# Additional Directories to deal with...
####################################################################################
#ADD_SUBDIRECTORY(plugins)

# INSTALL(TARGETS projectExe DESTINATION /usr/bin)
