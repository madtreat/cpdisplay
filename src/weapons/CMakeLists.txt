project(cpd-weapons)

set(SOURCES
   weaponscontroller.cpp
   weaponswidget.cpp
)

set(HEADERS
   weaponscontroller.h
   weaponswidget.h
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

install(TARGETS ${PROJECT_NAME} DESTINATION lib)

