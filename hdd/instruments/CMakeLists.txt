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
   #hdd-utils
   #hdd-core
)

qt5_use_modules(${PROJECT_NAME}
   Core
   Widgets
   Gui
   Svg
)
