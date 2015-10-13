project(cpd-config)

set(CONFIG
   maps.ini
   maps.js
   profile-1eng.ini
   profile-big-pfd.ini
   profile-default.ini
)

install(TARGETS ${CONFIG} DESTINATION config)

