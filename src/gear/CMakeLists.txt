project(cpd-gear)

set(SOURCES
   gearcontroller.cpp
   gearwidget.cpp
   singlegearwidget.cpp
)

set(HEADERS
   gearcontroller.h
   gearwidget.h
   singlegearwidget.h
)

add_library(${PROJECT_NAME} SHARED
   ${SOURCES}
   ${MOC_SRC}
)

target_link_libraries(${PROJECT_NAME}
   ${Qt5Core_LIBRARIES}
   ${Qt5Widgets_LIBRARIES}
   ${Qt5Gui_LIBRARIES}

   qt-toggle-input
   cpd-utils
   cpd-core
)
