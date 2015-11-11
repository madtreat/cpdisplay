project(cpd-map)

set(SOURCES
   mapcontroller.cpp
   mapwidget.cpp
   mapview.cpp
   mapoverlay.cpp
)

set(HEADERS
   mapconsts.h
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

target_link_libraries(${PROJECT_NAME} STATIC
   ${Qt5Core_LIBRARIES}
   ${Qt5Widgets_LIBRARIES}
   ${Qt5Gui_LIBRARIES}
   ${Qt5WebEngine_LIBRARIES}
   ${Qt5WebEngineWidgets_LIBRARIES}
   ${Qt5Svg_LIBRARIES}

   qt-gmaps

   cpd-utils
   #cpd-core
)

install(TARGETS ${PROJECT_NAME} DESTINATION lib)

