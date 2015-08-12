project(cpd-comms)

set(SOURCES
   commscontroller.cpp
   commsline.cpp
   commswidget.cpp
   timewidget.cpp
)

set(HEADERS
   commscontroller.h
   commsline.h
   commswidget.h
   timewidget.h
)

add_library(${PROJECT_NAME} SHARED
   ${SOURCES}
   ${MOC_SRC}
)

target_link_libraries(${PROJECT_NAME}
   ${Qt5Core_LIBRARIES}
   ${Qt5Widgets_LIBRARIES}
   ${Qt5Gui_LIBRARIES}

   #cpd-utils
   cpd-core
)
