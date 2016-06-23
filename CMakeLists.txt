cmake_minimum_required(VERSION 2.8.12)
project(cpd)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

list(APPEND CMAKE_MODULE_PATH ${cpd_SOURCE_DIR}/cmake_modules)

if(POLICY CMP0043)
cmake_policy(SET CMP0043 OLD)
endif()

################################################################################
# Version
################################################################################
set(cpd_MAJOR_VERSION 2)
set(cpd_MINOR_VERSION 4)
set(cpd_PATCH_VERSION 0)
set(cpd_VERSION ${cpd_MAJOR_VERSION}.${cpd_MINOR_VERSION}.${cpd_PATCH_VERSION})


################################################################################
# Set the output directory for the build executables and libraries
################################################################################
set(cpd_RUNTIME_OUTPUT_DIRECTORY ${cpd_SOURCE_DIR}/bin CACHE PATH "Target for the binaries")
if(WIN32)
  set(cpd_LIBRARY_OUTPUT_DIRECTORY ${cpd_SOURCE_DIR}/bin CACHE PATH "Target for the libraries")
else()
  set(cpd_LIBRARY_OUTPUT_DIRECTORY ${cpd_SOURCE_DIR}/lib CACHE PATH "Target for the libraries")
endif()
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${cpd_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${cpd_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${cpd_RUNTIME_OUTPUT_DIRECTORY})

set(CMAKE_INSTALL_PREFIX /opt/cpdisplay)
set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)
#set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


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
if(WIN32)
   add_definitions(-DWINDOWS)
   message(STATUS "Compiling on Windows")
   if(MSVC)
      message(STATUS "Compiling with MSVC")
      add_definitions(-DNOMINMAX)
      add_definitions(-D_USE_MATH_DEFINES)
      # SSE2 optimizations
      add_definitions("/arch:SSE2")
      if(BUILD_SHARED_LIBS)
         # disable warning on missing DLL interfaces
         add_definitions("/wd4251")
      endif()
   endif()
elseif(CYGWIN)
   message(STATUS "Compiling on Cygwin")
   add_definitions(-DCYGWIN)
elseif(APPLE)
   add_definitions(-DUNIX)
   message(STATUS "Compiling on OSX")
elseif(UNIX)
   add_definitions(-DUNIX)
   message(STATUS "Compiling on Unix")
endif()


################################################################################
# C++11 support
#
# Notes:
# OS X 10.8 Mountain Lion and above default to using the clang compiler,
# which does not use C++ 11 unless explicitly told to, as below.
################################################################################
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
   if(APPLE)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++ -O2")
   else()
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2")
   endif()
   message(STATUS "The compiler ${CMAKE_CXX_COMPILER} supports C++11")
elseif(COMPILER_SUPPORTS_CXX0X)
   if(APPLE)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -stdlib=libc++ -O2")
   else()
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -O2")
   endif()
   message(STATUS "The compiler ${CMAKE_CXX_COMPILER} supports C++0X")
else()
   message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()


################################################################################
# Add a couple custom targets to quickly make a debug or release version
################################################################################
set(CMAKE_BUILD_TYPE Release)

add_custom_target(debug
  COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug ${CMAKE_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
  COMMENT "Switch CMAKE_BUILD_TYPE to Debug"
)

add_custom_target(release
  COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release ${CMAKE_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
  COMMENT "Switch CMAKE_BUILD_TYPE to Release"
)


################################################################################
# Project Dependencies:
################################################################################
# Qt5 via Homebrew on OS X requires some fancy PREFIX_PATH changes
if(APPLE)
  set(CMAKE_PREFIX_PATH /usr/local/opt/qt5)
elseif(UNIX)
  set(CMAKE_PREFIX_PATH /opt/qt5/5.6/gcc_64)
endif()
find_package(Qt5Core    REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui     REQUIRED)
find_package(Qt5Svg     REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5DBus    REQUIRED)
find_package(Qt5Positioning      REQUIRED)
find_package(Qt5WebEngine        REQUIRED)
find_package(Qt5WebEngineWidgets REQUIRED)


################################################################################
# Variables
################################################################################
set(LIB_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/includes)
set(EXTERN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext)

link_directories(
   ${LIB_DIR}
)

include_directories(
   ${HEADER_DIR}
   ${EXTERN_DIR}
)


################################################################################
# Build the project and local dependencies
################################################################################
# Dependency: QFI (QFlightInstruments)
set(QFI_DIR ${EXTERN_DIR}/qfi)
set(QFI_CORE_HEADER_DIR ${QFI_DIR}/core)
set(QFI_UI_HEADER_DIR ${QFI_DIR}/ui)
include_directories(
   ${QFI_CORE_HEADER_DIR}
   ${QFI_UI_HEADER_DIR}
)
add_subdirectory(${QFI_DIR})

# Dependency: Qt Google Maps
# set(QTGMAPS_DIR ${EXTERN_DIR}/qt-maps)
# set(QTGMAPS_HEADER_DIR ${QTGMAPS_DIR})
# include_directories(
#    ${EXTERN_DIR}
# #   ${QTGMAPS_HEADER_DIR}
# )
# add_subdirectory(${QTGMAPS_DIR})

# Dependency: Qt Layout
set(QTLAYOUT_DIR ${EXTERN_DIR}/qt-dynamic-layout)
set(QTLAYOUT_HEADER_DIR ${QTLAYOUT_DIR})
#include_directories(
#   ${QTLAYOUT_HEADER_DIR}
#)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
add_subdirectory(${QTLAYOUT_DIR})

# Dependency: Qt Toggle Input
set(QTTOGGLEINPUT_DIR ${EXTERN_DIR}/qt-toggle-input)
set(QTTOGGLEINPUT_HEADER_DIR ${QTTOGGLEINPUT_DIR})
add_subdirectory(${QTTOGGLEINPUT_DIR})


# Project: CPD (Cockpit Display)
add_subdirectory(src)

# Project: CPD Configuration files and launch/utility scripts
# This is primarily for installation and package builds
add_subdirectory(config)
add_subdirectory(bin)
