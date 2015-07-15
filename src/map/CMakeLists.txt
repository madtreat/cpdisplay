project(hdd-map)

set(SOURCES
   mapcontroller.cpp
   mapwidget.cpp
   mapview.cpp
   mapoverlay.cpp
)

set(HEADERS
   mapcontroller.h
   mapwidget.h
   mapview.h
   mapoverlay.h
)

set(RESOURCES
   ../resources/resources.qrc
)
qt5_add_resources(QRC ${RESOURCES})
set_source_files_properties(${QRC} PROPERTIES GENERATED ON)


add_library(${PROJECT_NAME} SHARED
   ${SOURCES}
   ${MOC_SRC}
   ${QRC}
)

target_link_libraries(${PROJECT_NAME}
   ${Qt5Core_LIBRARIES}
   ${Qt5Widgets_LIBRARIES}
   ${Qt5Gui_LIBRARIES}
   ${Qt5WebKit_LIBRARIES}
   ${Qt5WebKitWidgets_LIBRARIES}
   ${Qt5Svg_LIBRARIES}

   qt-gmaps

   hdd-utils
   #hdd-core
)
