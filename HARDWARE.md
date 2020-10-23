# Hardware Installation

Note that all of the steps in this guide assume that the
MKR Relay Proto Shield is being used; if a different board
is being used, adjust the steps appropriately. This may
require adjusting settings in `config.hpp` if the sensors
are connected to different Arduino pins.

## Door Sensor

Install a 3.3KOhm resistor into the through-hole pads behind the
A1 screw terminal (or an SMT resistor onto the SMT pads).

Connect a suitable normally-open switch between the 3.3V supply
terminal and the A1 terminal.

## Door Control

Connect a two-conductor cable to the COM and NO screw terminals
for Relay 1, and connect the other end of the cable to the switch
terminals of the opener.

## Light Sensor

Install a 100KOhm resistor into the through-hole pads behind the
A2 screw terminal (or an SMT resistor onto the SMT pads).

Connect a suitable LDR between the 3.3V supply temrminal and the A2
terminal.
