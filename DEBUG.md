# Debugging

The startup process will emit messages as the components are
initalized, and then additional messages will be shown as the
components react to changes in sensor values, as MQTT commands are
received, and as the MQTT connection is lost and re-established.

## Serial Debugging

To enable serial debugging, set SERIAL_DEBUG in Makefile to a
non-empty value. Also ensure that GNU Screen is installed and
available via PATH.

When a SERIAL_DEBUG build is uploaded to the Arduino, Garaduino will
wait during startup until a serial connection is made. Use 'make
monitor' to open a serial connection. Use 'Ctrl-A K' to exit from
Screen; Garaduino will continue operating.

## Network Debugging

To enable network debugging, set NET_DEBUG in Makefile to a
non-empty value. Also ensure that Netcat is intalled and available
via PATH.

When a NET_DEBUG build is uploaded to the Arduino, Garaduino will
initialize its network connection and then wait for a network connection
to be made. Use 'make net_monitor' to open a network connection, and
'Ctrl-C' to exit from Netcat; Garaduino will continue operating.

Note that Garaduino can only accept one network connection per
startup; it is not possible to reconnect over the network without
restarting Garaduino.
