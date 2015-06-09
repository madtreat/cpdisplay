CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(hdd)

LIST(APPEND CMAKE_MODULE_PATH ${hdd_SOURCE_DIR}/cmake_modules)


################################################################################
# Version
################################################################################
SET(hdd_MAJOR_VERSION 0)
SET(hdd_MINOR_VERSION 1)
SET(hdd_PATCH_VERSION 0)
SET(hdd_VERSION ${hdd_MAJOR_VERSION}.${hdd_MINOR_VERSION}.${hdd_PATCH_VERSION})


################################################################################
# Set the output directory for the build executables and libraries
################################################################################
SET(hdd_RUNTIME_OUTPUT_DIRECTORY ${hdd_SOURCE_DIR}/bin CACHE PATH "Target for the binaries")
IF(WIN32)
   SET(hdd_LIBRARY_OUTPUT_DIRECTORY ${hdd_SOURCE_DIR}/bin CACHE PATH "Target for the libraries")
ELSE(WIN32)
   SET(hdd_LIBRARY_OUTPUT_DIRECTORY ${hdd_SOURCE_DIR}/lib CACHE PATH "Target for the libraries")
ENDIF(WIN32)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${hdd_LIBRARY_OUTPUT_DIRECTORY})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${hdd_LIBRARY_OUTPUT_DIRECTORY})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${hdd_RUNTIME_OUTPUT_DIRECTORY})


################################################################################
# Detect OS and define macros appropriately
################################################################################
IF(WIN32)
   ADD_DEFINITIONS(-DWINDOWS)
   MESSAGE(STATUS "Compiling on Windows")
   IF(MSVC)
      MESSAGE(STATUS "Compiling with MSVC")
      ADD_DEFINITIONS(-DNOMINMAX)
      ADD_DEFINITIONS(-D_USE_MATH_DEFINES)
      # SSE2 optimizations
      ADD_DEFINITIONS("/arch:SSE2")
      IF (BUILD_SHARED_LIBS)
         # disable warning on missing DLL interfaces
         ADD_DEFINITIONS("/wd4251")
      ENDIF()
   ENDIF(MSVC)
ELSEIF(CYGWIN)
   MESSAGE(STATUS "Compiling on Cygwin")
   ADD_DEFINITIONS(-DCYGWIN)
ELSEIF(APPLE)
   ADD_DEFINITIONS(-DUNIX)
   MESSAGE(STATUS "Compiling on OSX")
ELSEIF(UNIX)
   ADD_DEFINITIONS(-DUNIX)
   MESSAGE(STATUS "Compiling on Unix")
ENDIF(WIN32)


################################################################################
# C++11 support
#
# Notes:
# OS X 10.8 Mountain Lion and above default to using the clang compiler,
# which does not use C++ 11 unless explicitly told to, as below.
################################################################################
INCLUDE(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
IF(COMPILER_SUPPORTS_CXX11)
   IF(APPLE)
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++ -O2")
   ELSE(APPLE)
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2")
   ENDIF(APPLE)
   MESSAGE(STATUS "The compiler ${CMAKE_CXX_COMPILER} supports C++11")
ELSEIF(COMPILER_SUPPORTS_CXX0X)
   IF(APPLE)
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -stdlib=libc++ -O2")
   ELSE(APPLE)
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -O2")
   ENDIF(APPLE)
   MESSAGE(STATUS "The compiler ${CMAKE_CXX_COMPILER} supports C++0X")
ELSE()
   MESSAGE(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
ENDIF()


################################################################################
# Add a couple custom targets to quickly make a debug or release version
################################################################################
SET(CMAKE_BUILD_TYPE Release)

ADD_CUSTOM_TARGET(debug
  COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug ${CMAKE_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
  COMMENT "Switch CMAKE_BUILD_TYPE to Debug"
)

ADD_CUSTOM_TARGET(release
  COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release ${CMAKE_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
  COMMENT "Switch CMAKE_BUILD_TYPE to Release"
)


################################################################################
# Project Dependencies:
################################################################################
# Qt5 via Homebrew on OS X requires some fancy PREFIX_PATH changes
IF(APPLE)
   SET(CMAKE_PREFIX_PATH /usr/local/opt/qt5)
ENDIF(APPLE)
FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Gui REQUIRED)
#FIND_PACKAGE(Qt5OpenGL REQUIRED)


################################################################################
# Variables
################################################################################
SET(LIB_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
SET(HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/includes)
SET(EXTERN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext)

LINK_DIRECTORIES(
   ${LIB_DIR}
)

INCLUDE_DIRECTORIES(
   ${HEADER_DIR}
)


# Build the project
ADD_SUBDIRECTORY(hdd)

