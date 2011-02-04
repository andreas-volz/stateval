IF( COMMAND CMAKE_MINIMUM_REQUIRED )
        CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
ENDIF( COMMAND CMAKE_MINIMUM_REQUIRED )

####################################################################################
# The name of the project
####################################################################################
PROJECT(stateval)

# The base path for libsigc++
SET(LIBSIGCPP_DIR "${PROJECT_SOURCE_DIR}/../libsigc++-2.2.8")

# The base path for pluxx
SET(LIBPLUXX_DIR "${PROJECT_SOURCE_DIR}/../../pluxx")


####################################################################################
# Target OS specific flags.
####################################################################################
IF(APPLE)
    ADD_DEFINITIONS(-DDONT_USE_ECORE)
    FIND_LIBRARY(CPPUNIT cppunit)
    MARK_AS_ADVANCED (CPPUNIT)
    SET(EXTRA_LIBS ${CPPUNIT})
ELSE()
    SET(EXTRA_LIBS cppunit)
ENDIF (APPLE)

SET(EXTRA_LIBS pthread ${EXTRA_LIBS})

####################################################################################
# Make sure the compiler can find include files from our project.
####################################################################################
INCLUDE_DIRECTORIES (
    ${PROJECT_SOURCE_DIR}/include
    ${LIBSIGCPP_DIR}
) 

# build pluxx if there is a ${LIBPLUXX_DIR}/CMakeLists.txt
IF (EXISTS ${LIBPLUXX_DIR}/CMakeLists.txt)
    ADD_SUBDIRECTORY(${LIBPLUXX_DIR} pluxx)
ENDIF()

IF (EXISTS ${LIBSIGCPP_DIR}/CMakeLists.txt)
    ADD_SUBDIRECTORY(${LIBSIGCPP_DIR} libsigc++)
ENDIF()

INCLUDE_DIRECTORIES (${LIBPLUXX_DIR}/include)

####################################################################################
# The executeable(s) to build to.
####################################################################################
ADD_EXECUTABLE(testThread EXCLUDE_FROM_ALL
    test/module/testThread.cpp
)

ADD_SUBDIRECTORY(src)

SET(STATEVAL_LIBS z sigc++ stateval pluxx ${EXTRA_LIBS})

TARGET_LINK_LIBRARIES(testThread ${STATEVAL_LIBS})

####################################################################################
# Additional Directories to deal with...
####################################################################################
# INSTALL(TARGETS projectExe DESTINATION /usr/bin)

ADD_SUBDIRECTORY(test)
