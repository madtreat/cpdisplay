project(cpd-core)

set(SOURCES
   aircraft.cpp
   cpdsettings.cpp
   switchboard.cpp
   xplanedata.cpp
)

set(HEADERS
   aircraft.h
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
   ${Qt5Core_LIBRARIES}
   ${Qt5Widgets_LIBRARIES}
   ${Qt5Gui_LIBRARIES}
   ${Qt5Network_LIBRARIES}

   qfi-core
   qfi-ui

   cpd-utils
)

install(TARGETS ${PROJECT_NAME} DESTINATION lib)

