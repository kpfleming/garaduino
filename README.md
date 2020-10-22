# Garaduino

<a href="https://opensource.org"><img height="150" align="left" src="https://opensource.org/files/OSIApprovedCropped.png" alt="Open Source Initiative Approved License logo"></a>

Arduino sketch for a garage-monitoring system which supports a garage door,
light level sensing, and BLE beacon for the vehicle.

Compatible with Home Assistant, and inspired by [GarHAge](https://github.com/marthoc/GarHAge).

Open Source software: Apache License 2.0

## &nbsp;

## Overview

Garaduino is an Arduino sketch designed to connect Home Assistant (or any other
MQTT-enabled automation platform) to a garage, including the door, light level,
and vehicle presence. It supports Over the Air (OTA) updates as well.

## Features

## Requirements

### Hardware

* Any ARM-based (SAM core) Arduino should be sufficient for this sketch; as of the
current release the code requires less than 80KB of Flash and less than 10KB of RAM.
The sketch was developed using an Arduino MKR WiFi 1010.

   Note: ARM-based boards are required because this sketch is written using 'modern C++'
   features, and the AVR toolchains for Arduino boards do not support those features.

* An Ethernet interface supported by the standard Arduino Ethernet library. The sketch
was developd using an Arduino MKR Ethernet Shield.

* Connections to two analog pins, and a door control relay on a digital pin. The
sketch was developed using an Arduino MKR Relay Proto Shield.

* For vehicle presence detection, a Bluetooth interface supported by the standard
ArduinoBLE library, and a beacon (tested with a BlueCharm BC037). Note that the code
only uses the beacon's name; if the beacon also broadcasts data it is unused, and so
it does not matter which format the beacon uses for this data. The sketch was developed
using the Bluetooth support included in the MKR WiFi 1010.

## Hardware Configuration

### Door Sensor

A normally-open switch must be connected to an analog pin and to a voltage source compatible
with the analog input of the chosen board. In addition a pull-down resistor must be
used to hold the input 'low' when the switch is open.

The sketch was developed using a LittelFuse reed switch and magnet, and a 1/8W 3.3KOhm
resistor installed into the provided pads on the MKR Relay Proto Shield.

### Door Control

To activate a door opener, a normally-open relay must be connected to a digital pin.
The relay's load pins must be connected to the pins on the opener's control panel.

The sketch was developed using one of the relays on the MKR Relay Proto Shield, wired
to the control panel of a LiftMaster 8500W opener.

### Light Sensor

A light-dependent resistor (LDR) must be connected to an analog pin and to a voltage source
compatible with the analog input of the chosen board. In addition a pull-down resistor must be
used to hold the input 'low' when the LDR in its 'open' (dark) state.

The sketch was developed using an NTE LDR and a 1/8W 100KOhm resistor installed into the
provided pads on the MKR Relay Proto Shield. The high resistor value was chosen in order
to provide sufficiently wide range between 'light' and 'dark' states of the LDR.

### Vehicle Sensor

As noted above, a BLE beacon must be configured to broadcast a consistent name.

The sketch was developed using a BlueCharm BC037, which was configured using the
Eddystone Android app.

## Software Configuration and Build
