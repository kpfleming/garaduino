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

#include "ota.hpp"

#include "config.hpp"

#include <Ethernet.h>

#define NO_OTA_PORT
#include <ArduinoOTA.h>

#ifndef OTA_USERNAME
#define OTA_USERNAME "arduino"
#define OTA_PASSWORD "password"
#endif

namespace Garaduino {

void OTA::start(TimerSet& timers) {
    DEBUG_PRINT(F("Initializing OTA..."));

    ArduinoOTA.begin(::Ethernet.localIP(), OTA_USERNAME, OTA_PASSWORD, InternalStorage);

    DEBUG_PRINTLN(F(" done"));

    timers.every(OTA_POLL_SECS * 1000, [this]{ return maintain(); });
}

Timers::HandlerResult OTA::maintain() {
    ArduinoOTA.poll();

    return Timers::TimerStatus::repeat;
}

};
