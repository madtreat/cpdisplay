#!/bin/bash
#
# Requires FPM (gem install fpm)

TMP_INSTALL_DIR="/tmp/cpdisplay"

PKG_DIR="./packages"
if [ -d "$PKG_DIR" ]; then
   echo "Package build directory already exists, removing it for a clean install..."
   rm -rf "$PKG_DIR"
fi
mkdir "$PKG_DIR"
PKG_BUILD_DIR="$PKG_DIR/build"
# LOG_DIR is relative from $PKG_BUILD_DIR
LOG_DIR="../logs"


# Make clean build directory for the package
mkdir -p "$PKG_BUILD_DIR"
cd "$PKG_BUILD_DIR"
mkdir -p "$LOG_DIR"
echo "CMake..."
cmake ../.. > "$LOG_DIR/cmake.log" 2> "$LOG_DIR/cmake.error.log"
echo "Make..."
make -j9 > "$LOG_DIR/make.log" 2> "$LOG_DIR/make.error.log"

# Make the temp directory to hold the installation
mkdir -p "$TMP_INSTALL_DIR"
echo "Make install..."
make install DESTDIR="$TMP_INSTALL_DIR" > "$LOG_DIR/make.install.log" 2> "$LOG_DIR/make.install.error.log"


# Get the version from the executable
VERSION="$(../../bin/cpdisplay -v | cut -b 11-)"
echo "Version number: $VERSION"

# Go up a level and create the packages
cd ..
echo "Creating DEB package..."
fpm -s dir -t deb -a all -n vscl-cpdisplay -v $VERSION -C "$TMP_INSTALL_DIR" > "fpm.deb.log" 2> "fpm.deb.error.log"
echo "Creating RPM package..."
fpm -s dir -t rpm -a all -n vscl-cpdisplay -v $VERSION -C "$TMP_INSTALL_DIR" > "fpm.rpm.log" 2> "fpm.rpm.error.log"
cd -

echo "Done!"

