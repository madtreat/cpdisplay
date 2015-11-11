project(cpd-engine)

set(SOURCES
   enginecontroller.cpp
   enginedial.cpp
   enginewidget.cpp
   fuelwidget.cpp
   singleenginewidget.cpp
   singlefuelwidget.cpp
   throttlewidget.cpp
)

set(HEADERS
   engineconsts.h
   enginecontroller.h
   enginedial.h
   enginewidget.h
   fuelwidget.h
   singleenginewidget.cpp
   singlefuelwidget.h
   throttlewidget.h
)

add_library(${PROJECT_NAME} SHARED
   ${SOURCES}
   ${MOC_SRC}
)

target_link_libraries(${PROJECT_NAME} STATIC
   ${Qt5Core_LIBRARIES}
   ${Qt5Widgets_LIBRARIES}
   ${Qt5Gui_LIBRARIES}

   #cpd-utils
   cpd-core
)

install(TARGETS ${PROJECT_NAME} DESTINATION lib)

