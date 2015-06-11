PROJECT(qfi-ui)

SET(SOURCES
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

SET(HEADERS
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

SET(FORMS
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
QT5_WRAP_UI(FORMS_WRAPPED ${FORMS})

ADD_LIBRARY(${PROJECT_NAME} SHARED
   ${SOURCES}
   ${MOC_SRC}
   ${FORMS_WRAPPED}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
   ${Qt5Core_LIBRARIES}
   ${Qt5Widgets_LIBRARIES}
   ${Qt5Gui_LIBRARIES}
   ${Qt5Svg_LIBRARIES}
   qfi-core
)
