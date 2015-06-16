project(hdd-core)

set(SOURCES
   hddsettings.cpp
   switchboard.cpp
)

set(HEADERS
   hddsettings.h
   switchboard.h
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
#   hdd-utils
)
