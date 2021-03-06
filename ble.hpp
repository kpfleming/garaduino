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

#include "garaduino.hpp"
#include "web.hpp"
#include "mqtt.hpp"

namespace Garaduino {

class BLE {
public:
    BLE() = delete;
    BLE(TimerSet& timers, MQTT& mqtt) : timers(timers), mqtt(mqtt) {};
    ~BLE() {};

    void start(Web& web);

    // ensure that these objects will never be copied or moved
    // (this could only happen by accident)
    BLE(const BLE&) = delete;
    BLE& operator=(const BLE&) = delete;
    BLE(BLE&&) = delete;
    BLE& operator=(BLE&&) = delete;

private:
    TimerSet& timers;
    MQTT& mqtt;

    Timers::TimerHandle beacon_timer{};

    enum class State {
	unknown,
	absent,
	present
    };
    State lastState{State::unknown};
    String lastStateString{"unknown"};
    Web::statusItems statusItems{{ "Beacon", lastStateString }};

    void publishState(State state);
    void refresh();

    Timers::HandlerResult maintain();
    Timers::HandlerResult expire();
};

};
