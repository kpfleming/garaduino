# Garaduino

<a href="https://opensource.org"><img height="150" align="left"
src="https://opensource.org/files/OSIApprovedCropped.png" alt="Open
Source Initiative Approved License logo"></a>

Arduino sketch for a garage-monitoring system which supports a garage
door, light level sensing, and BLE beacon for the vehicle.

Compatible with Home Assistant, and inspired by
[GarHAge](https://github.com/marthoc/GarHAge).

Open Source software: Apache License 2.0

## &nbsp;

## Overview

Garaduino is an Arduino sketch designed to connect Home Assistant (or
any other MQTT-enabled automation platform) to a garage, including the
door, light level, and vehicle presence. It supports Over the Air
(OTA) updates as well.

## Features

Garaduino provides control of a garage door (via connection to its
opener) and status of the door; measurement of the outdoor light
level; and presence detection of a vehicle in the garage (using a BLE
beacon in the vehicle).

All of these features are supported via MQTT, and can be used with the
Home Assistant MQTT Cover and MQTT Sensor device configurations.

## Requirements

### Hardware

* Any ARM-based (SAM core) Arduino should be sufficient for this
sketch; as of the current release the code requires less than 80KB of
Flash and less than 10KB of RAM.  The sketch was developed using an
Arduino MKR WiFi 1010.

   Note: ARM-based boards are required because this sketch is written
   using 'modern C++' features, and the AVR toolchains for Arduino
   boards do not support those features.

* An Ethernet interface supported by the standard Arduino Ethernet
library. The sketch was developed using an Arduino MKR Ethernet
Shield.

* Connections to two analog pins, and a door control relay on a
digital pin. The sketch was developed using an Arduino MKR Relay Proto
Shield.

* For vehicle presence detection, a Bluetooth interface supported by
the standard ArduinoBLE library, and a beacon (tested with a BlueCharm
BC037). Note that the code only uses the beacon's name; if the beacon
also broadcasts data it is unused, and so it does not matter which
format the beacon uses for this data. The sketch was developed using
the Bluetooth support included in the MKR WiFi 1010.

### Software

* Arduino IDE. Required to obtain the toolchain (compiler/linker/etc.)
and libraries.  It is also used to bootstrap the board so that
Garaduino can be installed (see Installation section below). The IDE
can be installed using any available method; this sketch was developed
using version 1.8.13 but any recent version should work.

* Board package for your selected board. The simplest way to install
it is to open the Arduino IDE and then plug your board into the
computer via USB. The IDE will detect it and prompt you to allow the
installation of the necessary package. This sketch was developed using
version 1.8.9 of the SAM board package.

* Standard Arduino libraries
([Ethernet](https://github.com/arduino-libraries/Ethernet),
[ArduinoOTA](https://github.com/jandrassy/ArduinoOTA),
[ArduinoBLE](https://github.com/arduino-libraries/ArduinoBLE),
[PubSubClient](https://github.com/knolleary/pubsubclient)). The
simplest way to install them is to open the Arduino IDE and select
Tools->Manage Libraries from the menu, then find each one and click
the 'Install' button.

* [arduino-timer-cpp17](https://github.com/kpfleming/arduino-timer-cpp17)
library. Since this is not a standard library, it can't be installed
using the Manage Libraries tool. The simplest way to install it is to
go to the
[Tags](https://github.com/kpfleming/arduino-timer-cpp17/tags) page of
the repository on GitHub, and click the 'zip' link for the latest
version. Once the file has been downloaded, open the Arduino IDE and
click Sketch->Include Library->Add .ZIP Library, then navigate to the
downloaded file and install it. Note that you may have to update the
`ARDUINO_LIBS` variable in `Makefile` to include the version number of
the library that you downloaded.

* [Arduino-Makefile](https://github.com/sudar/Arduino-Makefile). Rather
than use the Arduino IDE for development, build, and upload, this
project uses the Arduino-Makefile kit to allow all of these operations
to be done from the command line. Use of this kit also eliminates the
need to modify the Arduino IDE toolchain configuration to support
C++17 mode in the compiler. GNU Make will also be required, and can be
installed using your operating system's normal software installation
tools. For monitoring the output of the Arduino (when a DEBUG build is
in place), GNU Screen will be required too. Clone this repository into
a suitable location.

* MQTT broker. An MQTT broker will be required to communicate with
Garaduino. The sketch was developed using the Mosquitto broker (which
is also the broker most commonly used with Home Assistant).

## Hardware Configuration

See [HARDWARE](HARDWARE.md) for connection details.

### Door Sensor

A normally-open switch must be connected to an analog pin and to a
voltage source compatible with the analog input of the chosen
board. In addition a pull-down resistor must be used to hold the input
'low' when the switch is open.

The sketch was developed using a LittelFuse reed switch and magnet,
and a 1/8W 3.3KOhm resistor installed into the provided pads on the
MKR Relay Proto Shield.

### Door Control

To activate a door opener, a normally-open relay must be connected to
a digital pin.  The relay's load pins must be connected to the pins on
the opener's control panel.

The sketch was developed using one of the relays on the MKR Relay
Proto Shield, wired to the control panel of a LiftMaster 8500W opener.

### Light Sensor

A light-dependent resistor (LDR) must be connected to an analog pin
and to a voltage source compatible with the analog input of the chosen
board. In addition a pull-down resistor must be used to hold the input
'low' when the LDR in its 'open' (dark) state.

The sketch was developed using an NTE 02-LDR2 and a 1/8W 100KOhm
resistor installed into the provided pads on the MKR Relay Proto
Shield. The high resistor value was chosen in order to provide
sufficiently wide range between 'light' and 'dark' states of the LDR.

### Vehicle Sensor

As noted above, a BLE beacon must be configured to broadcast a
consistent name.

The sketch was developed using a BlueCharm BC037, which was configured
using the Eddystone Android app.

## Software Configuration, Build, and Installation

See [BUILD](BUILD.md).
