project(hdd-pfd)

set(SOURCES
   pfdwidget.cpp
)

set(HEADERS
   pfdwidget.h
)


add_library(${PROJECT_NAME} SHARED
   ${SOURCES}
   ${MOC_SRC}
)

target_link_libraries(${PROJECT_NAME}
   hdd-utils
   hdd-core
)

qt5_use_modules(${PROJECT_NAME}
   Core
   Widgets
   Gui
)
