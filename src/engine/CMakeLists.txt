project(hdd-engine)

set(SOURCES
   enginecontroller.cpp
   enginedial.cpp
   enginedialwidget.cpp
   enginewidget.cpp
   throttlewidget.cpp
)

set(HEADERS
   engineconsts.h
   enginecontroller.h
   enginedial.h
   enginedialwidget.h
   enginewidget.h
   throttlewidget.h
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
   hdd-core
)
