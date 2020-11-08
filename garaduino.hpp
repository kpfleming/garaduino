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

#include <cstdint>

#include <arduino-timer-cpp17.hpp>

#if defined(SERIAL_DEBUG) && defined(NET_DEBUG)
#error Cannot define both SERIAL_DEBUG and NET_DEBUG.
#endif

#if defined(SERIAL_DEBUG) || defined(NET_DEBUG)
#ifdef SERIAL_DEBUG
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#else
#include <Client.h>
extern Client* debugClient;
#define DEBUG_PRINT(x) if ((debugClient != nullptr) && *debugClient) debugClient->print(x)
#define DEBUG_PRINTLN(x) if ((debugClient != nullptr) && *debugClient) debugClient->println(x)
#endif
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif

namespace Garaduino {

using TimerSet = Timers::TimerSet<10, Timers::Clock::millis>;

using PinNumber = std::uint8_t;
using PortNumber = std::uint16_t;

};
