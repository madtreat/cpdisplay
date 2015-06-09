project(hdd-map)

set(SOURCES
   mapwidget.cpp
)

set(HEADERS
   mapwidget.h
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
)
