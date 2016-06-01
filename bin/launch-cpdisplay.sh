#!/bin/bash
# 
# Launches the cpdisplay with the LD_LIBRARY_PATH set to the correct location
# to avoid having to make adjustments to each system to fix Qt installations.
#
# This script can also be used to set up a proxy if necessary.
#

# The directory containing this script
dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

host=`hostname -s`

# The executable's directory:
# IF this script is located in "/etc/cpdisplay/", then use the installed 
# executable in "/opt/cpdisplay".
# ELSE IF this script is located in a user's home directory, then use the
# local executable
if [ "$1" == "-l" ]; then
  exe="$dir/cpdisplay"

  # Get the proper settings file
  settings="$dir/../config/cpd-settings.ini"
else 
# elif [[ $dir == *"etc"* ]]; then
  exe="/opt/cpdisplay/bin/cpdisplay"

  # Get the proper settings file
  settings="/etc/cpdisplay/config/cpd-settings.$host.ini"
fi
echo "Script dir: $dir"
echo "Executable: $exe"

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

# The actual execution command
$exe -c $settings
