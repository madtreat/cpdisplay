project(hdd-core)

set(SOURCES
   aircraft.cpp
#   hddcontroller.cpp
   hddsettings.cpp
   switchboard.cpp
   xplanedata.cpp
)

set(HEADERS
   aircraft.h
   mapconsts.h
#   hddcontroller.h
   hddsettings.h
   switchboard.h
   xplanedata.h
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

   hdd-utils
#   hdd-comms
#   hdd-engine
#   hdd-instruments
#   hdd-map
#   hdd-traffic
)
