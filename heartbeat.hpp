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

namespace Garaduino {

class Heartbeat {
public:
    Heartbeat() = delete;
    Heartbeat(TimerSet& timers) : timers(timers) {};
    ~Heartbeat() {};

    void start();

    // ensure that these objects will never be copied or moved
    // (this could only happen by accident)
    Heartbeat(const Heartbeat&) = delete;
    Heartbeat& operator=(const Heartbeat&) = delete;
    Heartbeat(Heartbeat&&) = delete;
    Heartbeat& operator=(Heartbeat&&) = delete;

private:
    TimerSet& timers;

    Timers::HandlerResult heartbeat();
    Timers::HandlerResult setLED(int state);
};

};
