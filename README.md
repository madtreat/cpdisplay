CPDisplay
=========

Cockpit Head Down Display for the Vehicle Systems and Controls Lab at Texas A&M University


Dependencies
============
In addition to the dependencies listed below, you must have a Google Developer 
account with both 
[Google Maps JavaScript API](https://developers.google.com/maps/documentation/javascript/) and 
[Google Maps Geocoding API](https://developers.google.com/maps/documentation/geocoding/) enabled.

 Name                                       | Version     | Ubuntu Package  | OS X Homebrew Package | Notes
--------------------------------------------|-------------|-----------------|-----------------------|-------
[CMake](http://www.cmake.org/)              | 2.8.12+     | cmake           | cmake                 | 
[Qt5](http://qt-project.org/)               | 5.3+        | qt5-default, libqt5svg5, libqt5svg5-dev   | qt5      | 
[QFI](http://marekcel.pl/index.php?pg=qfi)  | 2014-01-26  | -               | -                     | included *1
[Qt Google Maps](https://github.tamu.edu:mtreat/qt-google-maps) | | - | - | included

1. QFI - QFlightInstruments
  * Used for the Primary Flight Display and other instruments


Instructions
============
The easiest way to download and build this project is using Git and CMake
(after installing non-included dependencies):
```bash
$ git clone https://github.tamu.edu/vscl/cpdisplay.git
$ git submodule init
$ git submodule update
$ mkdir build && cd build
$ cmake ..
$ make
## Optionally run from this directory
$ ../bin/[executable]
```

The executable can be run from anywhere, as long as the config directory exists
in the same structural location on the file system ( [project root]/bin/executable
and [project root]/config/[files] )