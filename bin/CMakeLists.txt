project(cpd-bin-scripts)

set(CONFIG
  launch-cpdisplay.sh
  mcsds.service
)

install(FILES ${CONFIG} DESTINATION bin)

