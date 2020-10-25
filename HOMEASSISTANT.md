# Home Assistant

Garaduino supports MQTT messages which conform to the Home Assistant
MQTT Cover (garage door), MQTT Sensor (light level), and MQTT Binary
Sensor (vehicle presence) integrations.

## Example Configuration

```yaml
cover:
  - platform: mqtt
    name: "Garage Door"
    state_topic: "garaduino/door/status"
    command_topic: "garaduino/door/control"
    availability_topic: "garaduino/availability"

sensor:
  - platform: mqtt
    name: "Light Level"
    state_topic: "garaduino/light"
    availability_topic: "garaduino/availability"
    device_class: illuminance

binary_sensor:
  - platform: mqtt
    name: "Vehicle"
    state_topic: "garaduino/vehicle"
    payload_on: "present"
    payload_off: "absent"
    availability_topic: "garaduino/availability"
    device_class: presence
```
