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

#include <vector>

#include "garaduino.hpp"
#include "mqtt.hpp"

namespace Garaduino {

class Light {
public:
    struct stateMapEntry {
	int level;
	const char* state;
    };

    using stateMap = std::vector<stateMapEntry>;

    Light(TimerSet& timers, MQTT& mqtt, const stateMap& map, PinNumber sensor) : timers(timers), mqtt(mqtt), map(map), sensor(sensor) {};
    ~Light() {};

    void start();

    // ensure that these objects will never be copied or moved
    // (this could only happen by accident)
    Light() = delete;
    Light(const Light&) = delete;
    Light& operator=(const Light&) = delete;
    Light(Light&&) = delete;
    Light& operator=(Light&&) = delete;

private:
    TimerSet& timers;
    MQTT& mqtt;
    const stateMap& map;
    PinNumber sensor;

    stateMapEntry lastStateMapEntry{};
    const stateMapEntry& getStateMapEntry();
    void publishState(const stateMapEntry& entry);

    Timers::HandlerResult maintain();
};

};
