project(cpd-core)

set(SOURCES
  aircraft.cpp
  cpdsettings.cpp
  switchboard.cpp
  xplanedata.cpp
)

set(HEADERS
  aircraft.h
  coreconsts.h
  cpdsettings.h
  switchboard.h
  xplanedata.h
  xplanedref.h
)


add_library(${PROJECT_NAME} SHARED
  ${SOURCES}
  ${MOC_SRC}
)

target_link_libraries(${PROJECT_NAME}
  Qt5::Core
  Qt5::Network

  cpd-utils
)

install(TARGETS ${PROJECT_NAME} DESTINATION lib)

