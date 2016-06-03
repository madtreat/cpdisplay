project(mcsdataswitch)

set(SOURCES
  mcsdataswitch.cpp
)

set(HEADERS
  mcsdataswitch.h
)


add_executable(${PROJECT_NAME}
  main.cpp
  ${SOURCES}
  ${MOC_SRC}
)

target_link_libraries(${PROJECT_NAME}
  Qt5::Core
  Qt5::Network
  cpd-core
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

