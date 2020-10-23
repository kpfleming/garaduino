// SPDX-FileCopyrightText: 2020 Kevin P. Fleming <kevin@km6g.us>
// SPDX-License-Identifier: Apache-2.0

// Copyright 2020 Kevin P. Fleming
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

const Timers::Timepoint HEARTBEAT_SECS{10};
const Timers::Timepoint HEARTBEAT_PULSE_MSECS{250};

const PinNumber LIGHT_SENSOR{A2};
const Timers::Timepoint LIGHT_SENSOR_POLL_SECS{30};
const char MQTT_LIGHT_TOPIC[]{"garaduino/light"};

const PinNumber DOOR_CONTROL{1};
const Timers::Timepoint DOOR_CONTROL_MSECS{100};
const PinNumber DOOR_SENSOR{A1};
const Timers::Timepoint DOOR_SENSOR_POLL_SECS{1};
const Timers::Timepoint DOOR_SENSOR_DEBOUNCE_MSECS{5000};
const char MQTT_DOOR_CONTROL_TOPIC[]{"garaduino/door/control"};
const char MQTT_DOOR_SENSOR_TOPIC[]{"garaduino/door/status"};

const Timers::Timepoint ETHERNET_POLL_SECS{60};

const Timers::Timepoint OTA_POLL_SECS{2};

const char MQTT_BROKER_NAME[]{"192.168.64.108"};
const PortNumber MQTT_BROKER_PORT{1883};
const char MQTT_CLIENT_NAME[]{"garaduino"};
const Timers::Timepoint MQTT_POLL_SECS{5};
const char MQTT_STATUS_TOPIC[]{"garaduino/status"};
const char MQTT_VERSION_TOPIC[]{"garaduino/version"};

const char BLE_BEACON_NAME[]{"km6gauto"};
const Timers::Timepoint BLE_POLL_SECS{5};
const Timers::Timepoint BLE_BEACON_TIMEOUT_SECS{60};
const char MQTT_BEACON_TOPIC[]{"garaduino/vehicle"};
