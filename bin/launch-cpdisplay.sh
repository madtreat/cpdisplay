#!/bin/bash
# 
# Launches the cpdisplay with the LD_LIBRARY_PATH set to the correct location
# to avoid having to make adjustments to each system to fix Qt installations.

# The directory containing this script
dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# The executable's directory:
# IF this script is located in "/etc/cpdisplay/", then use the installed 
# executable in "/opt/cpdisplay".
# ELSE IF this script is located in a user's home directory, then use the
# local executable
if [[ $dir == *"etc"* ]]; then
  exe="/opt/cpdisplay/bin/cpdisplay"
elif [[ $dir == *"home"* ]]; then
  exe="$dir/cpdisplay"
fi

# Set up the LD_LIBRARY_PATH because Qt installations are weird and often
# do not link the QtWebEngineProcess to the Qt core libs properly.
# This prevents the need to fix every machine's linking individually.
qt_install_path="/opt/qt/5.6/gcc_64"
export LD_LIBRARY_PATH="$qt_install_path/lib"

# Set up the proxy
proxy_host="vsclproxy.ipa.vscl.tamu.edu"
proxy_port="3128"
export http_proxy="$proxy_host:$proxy_port"
export https_proxy="$proxy_host:$proxy_port"

# Get the proper settings file
host=`hostname -s`
settings="/etc/cpdisplay/config/cpd-settings.$host.ini"

# The actual execution command
$exe -c $settings
