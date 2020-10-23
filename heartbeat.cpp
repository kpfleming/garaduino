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

#include "heartbeat.hpp"

#include "config.hpp"

namespace Garaduino {

void Heartbeat::start() {
    pinMode(LED_BUILTIN, OUTPUT);

    timers.now_and_every(HEARTBEAT_SECS * 1000, [this]{ return heartbeat(); });
}

Timers::HandlerResult Heartbeat::heartbeat() {
    DEBUG_PRINTLN(F("Heartbeat"));

    setLED(HIGH);
    timers.in(HEARTBEAT_PULSE_MSECS, [this]{ return setLED(LOW); });

    return Timers::TimerStatus::repeat;
}

Timers::HandlerResult Heartbeat::setLED(int state) {
    digitalWrite(LED_BUILTIN, state);

    return Timers::TimerStatus::completed;
}

};
