project(cpdisplay)

set(SOURCES
   cpdcontroller.cpp
   cpdwidget.cpp
   cpdwindow.cpp
   mcsslavewidget.cpp
   mcswidget.cpp
)

set(HEADERS
   cpdcontroller.h
   cpdwidget.h
   cpdwindow.h
   mcsslavewidget.h
   mcswidget.h
)

set(RESOURCES
   ../resources/resources.qrc
)
qt5_add_resources(QRC ${RESOURCES})
set_source_files_properties(${QRC} PROPERTIES GENERATED ON)


add_executable(${PROJECT_NAME}
   main.cpp
   ${SOURCES}
   ${MOC_SRC}
   ${QRC}
)

target_link_libraries(${PROJECT_NAME}
   ${Qt5Core_LIBRARIES}
   ${Qt5Widgets_LIBRARIES}
   ${Qt5Gui_LIBRARIES}
   ${Qt5DBus_LIBRARIES}
   ${Qt5XcbQpa_LIBRARIES}

   qfi-core
   qfi-ui
   qt-layout

   cpd-utils
   cpd-core
   cpd-instruments
   cpd-map
   cpd-gear
   cpd-comms
   cpd-engine
   cpd-traffic
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

