IF( COMMAND CMAKE_MINIMUM_REQUIRED )
        CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
ENDIF( COMMAND CMAKE_MINIMUM_REQUIRED )

####################################################################################
# The name of the project
####################################################################################
PROJECT(TextViews)

# The base path for pluxx
SET(STATEVAL_DIR "${PROJECT_SOURCE_DIR}/../../../stateval")

# The base path for pluxx
SET(LIBPLUXX_DIR "${PROJECT_SOURCE_DIR}/../../../../pluxx")

# The base path for libsigc++
SET(LIBSIGCPP_DIR "${PROJECT_SOURCE_DIR}/../../../libsigc++-2.2.8")

####################################################################################
# Make sure the compiler can find include files from our project.
####################################################################################
INCLUDE_DIRECTORIES (
    ${STATEVAL_DIR}/include
    ${LIBPLUXX_DIR}/include
    ${LIBSIGCPP_DIR}/include
    ${PROJECT_SOURCE_DIR}
    ${LIBSIGCPP_DIR}
    ) 

####################################################################################
# The executeable(s) to build to.
####################################################################################
ADD_EXECUTABLE(text_views
    Main.cpp
    Main.h
    InputThread.cpp
    InputThread.h
    searchFile.cpp
    searchFile.h
)

TARGET_LINK_LIBRARIES(text_views stateval pluxx sigc++)

####################################################################################
# Additional Directories to deal with...
####################################################################################
ADD_SUBDIRECTORY(../.. stateval)

# INSTALL(TARGETS projectExe DESTINATION /usr/bin)
