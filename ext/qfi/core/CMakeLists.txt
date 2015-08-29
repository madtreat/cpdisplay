PROJECT(qfi-core)

SET(SOURCES
   qfi_ADI.cpp
   qfi_ALT.cpp
   qfi_ASI.cpp
   qfi_HSI.cpp
   qfi_PFD.cpp
   qfi_TC.cpp
   qfi_VSI.cpp
)

SET(HEADERS
   qfi_ADI.h
   qfi_ALT.h
   qfi_ASI.h
   qfi_HSI.h
   qfi_PFD.h
   qfi_TC.h
   qfi_VSI.h
)

# Set up the images as resources
SET(RESOURCES
   ${QFI_DIR}/qfi.qrc
)
QT5_ADD_RESOURCES(QFI_IMAGES ${RESOURCES})
SET_SOURCE_FILES_PROPERTIES(${QFI_IMAGES} PROPERTIES GENERATED ON)

ADD_LIBRARY(${PROJECT_NAME} SHARED
   ${SOURCES}
   ${MOC_HEADERS}
   ${QFI_IMAGES}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
   ${QT_LIBRARIES}
)

qt5_use_modules(${PROJECT_NAME}
   Core
   Widgets
   Svg
)

install(TARGETS ${PROJECT_NAME} DESTINATION lib)

