project(qfi-ui)

set(SOURCES
   LayoutSquare.cpp
   MainWindow.cpp
   WidgetADI.cpp
   WidgetALT.cpp
   WidgetASI.cpp
   WidgetHSI.cpp
   WidgetPFD.cpp
   WidgetSix.cpp
   WidgetTC.cpp
   WidgetVSI.cpp
)

set(HEADERS
   LayoutSquare.h
   MainWindow.h
   WidgetADI.h
   WidgetALT.h
   WidgetASI.h
   WidgetHSI.h
   WidgetPFD.h
   WidgetSix.h
   WidgetTC.h
   WidgetVSI.h
)

set(FORMS
   MainWindow.ui
   WidgetADI.ui
   WidgetALT.ui
   WidgetASI.ui
   WidgetHSI.ui
   WidgetPFD.ui
   WidgetSix.ui
   WidgetTC.ui
   WidgetVSI.ui
)
qt5_wrap_ui(FORMS_WRAPPED ${FORMS})

add_library(${PROJECT_NAME} SHARED
   ${SOURCES}
   ${MOC_SRC}
   ${FORMS_WRAPPED}
)

target_link_libraries(${PROJECT_NAME}
   ${Qt5Core_LIBRARIES}
   ${Qt5Widgets_LIBRARIES}
   ${Qt5Gui_LIBRARIES}
   ${Qt5Svg_LIBRARIES}
   qfi-core
)

# For testing purposes, it is easy to just add an additional executable to the build
add_executable(qfi-test
   main.cpp
)

target_link_libraries(qfi-test
   ${PROJECT_NAME}
)
