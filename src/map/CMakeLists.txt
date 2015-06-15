project(hdd-map)

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


add_library(${PROJECT_NAME} SHARED
   ${SOURCES}
   ${MOC_SRC}
)

target_link_libraries(${PROJECT_NAME}
   ${Qt5Core_LIBRARIES}
   ${Qt5Widgets_LIBRARIES}
   ${Qt5Gui_LIBRARIES}

   qt-gmaps

   #hdd-utils
   #hdd-core
)
