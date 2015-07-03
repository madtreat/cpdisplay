project(hdd-utils)

set(SOURCES
   geodist.cpp
)

set(HEADERS
   geodist.h
)


add_library(${PROJECT_NAME} SHARED
   ${SOURCES}
   ${MOC_SRC}
)

#target_link_libraries(${PROJECT_NAME}
#   ${Qt5Core_LIBRARIES}
#)
