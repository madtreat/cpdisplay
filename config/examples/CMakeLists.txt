project(cpd-config-examples)

set(CONFIG
   cpd-settings.ini
   google-maps.js
   ol-map.ini
   ol-map.js
   profile.ini
   style-default.qss
)

install(FILES ${CONFIG} DESTINATION config/examples)

