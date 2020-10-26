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

#include "light.hpp"

#include "config.hpp"

namespace Garaduino {

void Light::start() {
    timers.now_and_every(LIGHT_SENSOR_POLL_SECS * 1000, [this]{ return maintain(); });
}

const Light::stateMapEntry& Light::getStateMapEntry() {
    int sense = analogRead(LIGHT_SENSOR);

    for (auto& entry: map) {
	if (sense <= entry.level) {
	    return entry;
	}
    }

    return map.back();
}

void Light::publishState(const stateMapEntry& entry) {
    DEBUG_PRINT(F("Light level is "));
    DEBUG_PRINTLN(entry.state);
    mqtt.publish(MQTT_LIGHT_TOPIC, entry.state, true);
}

Timers::HandlerResult Light::maintain() {
    const stateMapEntry& current{getStateMapEntry()};

    if (current.state != lastStateMapEntry.state) {
	publishState(current);
	lastStateMapEntry = current;
    }

    return Timers::TimerStatus::repeat;
}

};
