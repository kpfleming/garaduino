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

#include "garaduino.hpp"

#include "gitversion.hpp"

#include "heartbeat.hpp"
#include "ethernet.hpp"
#include "ota.hpp"
#include "mqtt.hpp"
#include "ble.hpp"
#include "light.hpp"
#include "door.hpp"

#include "config.hpp"

namespace {

Garaduino::TimerSet timers;

Garaduino::Heartbeat heartbeat{timers};

Garaduino::Ethernet ethernet{timers, Garaduino::ETHERNET_MAC};

Garaduino::OTA ota{timers};

Garaduino::MQTT mqtt{timers};

Garaduino::BLE ble{timers, mqtt};

Garaduino::Light light{timers, mqtt, Garaduino::LIGHT_STATE_MAP, Garaduino::LIGHT_SENSOR};

Garaduino::Door door{timers, mqtt, Garaduino::DOOR_CONTROL, Garaduino::DOOR_SENSOR};

};

void setup() {
#ifdef DEBUG
    Serial.begin(9600);
    while(!Serial);
#endif

    DEBUG_PRINTLN(F("Garaduino initializing"));
    DEBUG_PRINT(F("Version: "));
    DEBUG_PRINTLN(GIT_VERSION);

    heartbeat.start();
    ethernet.start();
    ota.start();
    mqtt.start();
    mqtt.publish(Garaduino::MQTT_VERSION_TOPIC, GIT_VERSION, true);
    ble.start();
    light.start();
    door.start();

    DEBUG_PRINTLN(F("Garaduino started"));
}

void loop() {
    timers.tick_and_delay();
}
