CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)
PROJECT(cpd)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

LIST(APPEND CMAKE_MODULE_PATH ${cpd_SOURCE_DIR}/cmake_modules)

IF(POLICY CMP0043)
CMAKE_POLICY(SET CMP0043 OLD)
ENDIF(POLICY CMP0043)

################################################################################
# Version
################################################################################
SET(cpd_MAJOR_VERSION 2)
SET(cpd_MINOR_VERSION 2)
SET(cpd_PATCH_VERSION 1)
SET(cpd_VERSION ${cpd_MAJOR_VERSION}.${cpd_MINOR_VERSION}.${cpd_PATCH_VERSION})


################################################################################
# Set the output directory for the build executables and libraries
################################################################################
SET(cpd_RUNTIME_OUTPUT_DIRECTORY ${cpd_SOURCE_DIR}/bin CACHE PATH "Target for the binaries")
IF(WIN32)
   SET(cpd_LIBRARY_OUTPUT_DIRECTORY ${cpd_SOURCE_DIR}/bin CACHE PATH "Target for the libraries")
ELSE(WIN32)
   SET(cpd_LIBRARY_OUTPUT_DIRECTORY ${cpd_SOURCE_DIR}/lib CACHE PATH "Target for the libraries")
ENDIF(WIN32)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${cpd_LIBRARY_OUTPUT_DIRECTORY})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${cpd_LIBRARY_OUTPUT_DIRECTORY})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${cpd_RUNTIME_OUTPUT_DIRECTORY})

SET(CMAKE_INSTALL_PREFIX /opt/cpdisplay)
SET(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)
#SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


################################################################################
# Set the output directory for the build executables and libraries
################################################################################
set(CPACK_PACKAGE_VERSION ${cpd_VERSION})
#set(CPACK_GENERATOR "RPM")
set(CPACK_GENERATOR "RPM")
set(CPACK_PACKAGE_NAME "cpdisplay")
set(CPACK_PACKAGE_RELEASE 1)
set(CPACK_PACKAGE_CONTACT "Madison Treat")
set(CPACK_PACKAGE_VENDOR "VSCL")
set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")
include(CPack)


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
FIND_PACKAGE(Qt5Core    REQUIRED)
FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Gui     REQUIRED)
FIND_PACKAGE(Qt5Svg     REQUIRED)
FIND_PACKAGE(Qt5Network REQUIRED)
FIND_PACKAGE(Qt5Positioning      REQUIRED)
FIND_PACKAGE(Qt5WebEngine        REQUIRED)
FIND_PACKAGE(Qt5WebEngineWidgets REQUIRED)


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


################################################################################
# Build the project and local dependencies
################################################################################
# Dependency: QFI (QFlightInstruments)
SET(QFI_DIR ${EXTERN_DIR}/qfi)
SET(QFI_CORE_HEADER_DIR ${QFI_DIR}/core)
SET(QFI_UI_HEADER_DIR ${QFI_DIR}/ui)
INCLUDE_DIRECTORIES(
   ${QFI_CORE_HEADER_DIR}
   ${QFI_UI_HEADER_DIR}
)
ADD_SUBDIRECTORY(${QFI_DIR})

# Dependency: Qt Google Maps
SET(QTGMAPS_DIR ${EXTERN_DIR}/qt-google-maps)
SET(QTGMAPS_HEADER_DIR ${QTGMAPS_DIR})
INCLUDE_DIRECTORIES(
   ${EXTERN_DIR}
#   ${QTGMAPS_HEADER_DIR}
)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
ADD_SUBDIRECTORY(${QTGMAPS_DIR})

# Dependency: Qt Layout
SET(QTLAYOUT_DIR ${EXTERN_DIR}/qt-layout)
SET(QTLAYOUT_HEADER_DIR ${QTLAYOUT_DIR})
#INCLUDE_DIRECTORIES(
#   ${QTLAYOUT_HEADER_DIR}
#)
ADD_SUBDIRECTORY(${QTLAYOUT_DIR})

# Dependency: Qt Toggle Input
SET(QTTOGGLEINPUT_DIR ${EXTERN_DIR}/qt-toggle-input)
SET(QTTOGGLEINPUT_HEADER_DIR ${QTTOGGLEINPUT_DIR})
ADD_SUBDIRECTORY(${QTTOGGLEINPUT_DIR})


# Project: CPD (Cockpit Display)
ADD_SUBDIRECTORY(src)

# Project: CPD Configuration files
# This is primarily for installation and package builds
ADD_SUBDIRECTORY(config)

