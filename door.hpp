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
#include "mqtt.hpp"

namespace Garaduino {

class Door {
public:
    Door(TimerSet& timers, MQTT& mqtt, PinNumber control, PinNumber sensor) : timers(timers), mqtt(mqtt), control(control), sensor(sensor) {};
    ~Door() {};

    void start();

    // ensure that these objects will never be copied or moved
    // (this could only happen by accident)
    Door() = delete;
    Door(const Door&) = delete;
    Door& operator=(const Door&) = delete;
    Door(Door&&) = delete;
    Door& operator=(Door&&) = delete;

private:
    TimerSet& timers;
    MQTT& mqtt;
    PinNumber control;
    PinNumber sensor;

    enum class State {
	unknown,
	open,
	closed
    };
    State lastState{State::unknown};
    unsigned long debounceStartTime{0};
    State getState();
    void publishState(State state);
    void refresh();

    Timers::HandlerResult setControl(int state);

    Timers::HandlerResult maintain();

    void handleControlMessage(const String& message);

    void triggerOpener();
};

};
