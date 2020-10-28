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

#include "ble.hpp"

#include "config.hpp"

#include <ArduinoBLE.h>

namespace Garaduino {

void BLE::start() {
    DEBUG_PRINT(F("Initializing BLE..."));

    if (!::BLE.begin()) {
	DEBUG_PRINTLN(F(" failed."));
	while (true);
    }

    ::BLE.scan(true);

    DEBUG_PRINTLN(F(" done"));

    timers.every(BLE_POLL_SECS * 1000, [this]{ return maintain(); });
    // setup an initial state publication if beacon is not seen
    beacon_timer = timers.in(BLE_BEACON_TIMEOUT_SECS * 1000, [this]{ return expire(); });
}

Timers::HandlerResult BLE::maintain() {
    ::BLE.poll();

    while (auto peripheral = ::BLE.available()) {
	if (peripheral.hasLocalName()) {
	    auto name{peripheral.localName()};

	    if (name.equals(BLE_BEACON_NAME)) {
		if (beacon_timer) {
		    timers.reschedule_in(beacon_timer, BLE_BEACON_TIMEOUT_SECS * 1000);
		} else if (lastState != State::present) {
		    publishState(State::present);
		    lastState = State::present;
		    beacon_timer = timers.in(BLE_BEACON_TIMEOUT_SECS * 1000, [this]{ return expire(); });
		}
	    }
	}
    }

    return Timers::TimerStatus::repeat;
}

void BLE::publishState(State state) {
    String stateString;

    switch (state) {
    case State::absent:
	stateString = "absent";
	break;
    case State::present:
	stateString = "present";
	break;
    case State::unknown:
	stateString = "unknown";
	break;
    }

    DEBUG_PRINT(F("Beacon is "));
    DEBUG_PRINTLN(stateString);
    mqtt.publishAndRetain(MQTT_BEACON_TOPIC, stateString);
}

Timers::HandlerResult BLE::expire() {
    if (lastState != State::absent) {
	publishState(State::absent);
	lastState = State::absent;
    }

    beacon_timer = Timers::TimerHandle{};

    return Timers::TimerStatus::completed;
}

};
