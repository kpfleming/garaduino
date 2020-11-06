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

void BLE::start(Web& web) {
    DEBUG_PRINT(F("Initializing BLE..."));

    if (!::BLE.begin()) {
	DEBUG_PRINTLN(F(" failed."));
	while (true);
    }

    ::BLE.scan(true);

    DEBUG_PRINTLN(F(" done"));

    mqtt.subscribe(MQTT_REFRESH_TOPIC, [this](const String &) { return refresh(); });

    web.addStatusItemProvider("ble", [this]()->auto& { return statusItems; });

    timers.every(BLE_POLL_SECS * 1000, [this] { return maintain(); });
    // setup an initial state publication if beacon is not seen
    beacon_timer = timers.in(BLE_BEACON_TIMEOUT_SECS * 1000, [this] { return expire(); });
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
    switch (state) {
    case State::absent:
	lastStateString = "absent";
	break;
    case State::present:
	lastStateString = "present";
	break;
    case State::unknown:
	lastStateString = "unknown";
	break;
    }

    DEBUG_PRINT(F("Beacon is "));
    DEBUG_PRINTLN(lastStateString);
    mqtt.publishAndRetain(MQTT_BEACON_TOPIC, lastStateString);
}

Timers::HandlerResult BLE::expire() {
    if (lastState != State::absent) {
	publishState(State::absent);
	lastState = State::absent;
    }

    beacon_timer = Timers::TimerHandle{};

    return Timers::TimerStatus::completed;
}

void BLE::refresh() {
    DEBUG_PRINTLN(F("BLE refresh"));
    publishState(lastState);
}

};
