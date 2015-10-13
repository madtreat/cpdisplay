project(cpd-config)

add_subdirectory(examples)

set(CONFIG
   maps.ini
   maps.js
   profile-1eng.ini
   profile-big-pfd.ini
   profile-default.ini
)

install(FILES ${CONFIG} DESTINATION config)

