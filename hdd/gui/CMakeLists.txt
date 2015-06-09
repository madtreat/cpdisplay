project(hdd-gui)

set(SOURCES
   window.cpp
)

set(HEADERS
   window.h
)


add_executable(${PROJECT_NAME}
   main.cpp
   ${SOURCES}
   ${MOC_SRC}
)

target_link_libraries(${PROJECT_NAME}
   qfi-core
   qfi-ui

   hdd-utils
   hdd-core
   hdd-pfd
   hdd-map
)

qt5_use_modules(${PROJECT_NAME}
   Core
   Widgets
   Gui
)
