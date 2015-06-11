project(hdd-map)

set(SOURCES
   mapcontroller.cpp
)

set(HEADERS
   mapcontroller.h
)


add_library(${PROJECT_NAME} SHARED
   ${SOURCES}
   ${MOC_SRC}
)

target_link_libraries(${PROJECT_NAME}
   ${Qt5Core_LIBRARIES}
   ${Qt5Widgets_LIBRARIES}
   ${Qt5Gui_LIBRARIES}

   #hdd-utils
   #hdd-core
)
