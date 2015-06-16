project(hdd-gui)

set(SOURCES
   hddcontroller.cpp
   window.cpp
)

set(HEADERS
   hddcontroller.h
   window.h
)


add_executable(${PROJECT_NAME}
   main.cpp
   ${SOURCES}
   ${MOC_SRC}
)

target_link_libraries(${PROJECT_NAME}
   ${Qt5Core_LIBRARIES}
   ${Qt5Widgets_LIBRARIES}
   ${Qt5Gui_LIBRARIES}

   qfi-core
   qfi-ui

   #hdd-utils
   hdd-core
   hdd-instruments
   hdd-map
)
