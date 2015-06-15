project(hdd-instruments)

set(SOURCES
   adicontroller.cpp
   altcontroller.cpp
   asicontroller.cpp
   hsicontroller.cpp
   pfdcontroller.cpp
   tcdcontroller.cpp
   vsicontroller.cpp
)

set(HEADERS
   adicontroller.h
   altcontroller.h
   asicontroller.h
   hsicontroller.h
   pfdcontroller.h
   tcdcontroller.h
   vsicontroller.h
)


add_library(${PROJECT_NAME} SHARED
   ${SOURCES}
   ${MOC_SRC}
)

target_link_libraries(${PROJECT_NAME}
   ${Qt5Core_LIBRARIES}
   ${Qt5Widgets_LIBRARIES}
   ${Qt5Gui_LIBRARIES}
   ${Qt5Svg_LIBRARIES}

   qfi-core
   qfi-ui
   #hdd-utils
   #hdd-core
)
