project(cpd-config-examples)

set(CONFIG
   cpd-settings.ini
   google-maps.js
   maps.ini
   ol-map.js
   profile.ini
   qt-google-maps.ini
   style-default.qss
)

install(TARGETS ${CONFIG} DESTINATION config/examples)

