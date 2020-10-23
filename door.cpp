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

#include "door.hpp"

#include "config.hpp"

namespace Garaduino {

void Door::start() {
    pinMode(control, OUTPUT);
    digitalWrite(control, LOW);

    mqtt.subscribe(MQTT_DOOR_CONTROL_TOPIC, [this](const String& msg){ return handleControlMessage(msg); });

    timers.now_and_every(DOOR_SENSOR_POLL_SECS * 1000, [this]{ return maintain(); });
}

auto Door::getState() -> State {
    auto sense{analogRead(sensor)};

    return sense > 100 ? State::closed : State::open;
}

void Door::publishState(State state) {
    String stateString;

    switch (state) {
    case State::closed:
	stateString = "closed";
	break;
    case State::open:
	stateString = "open";
	break;
    case State::unknown:
	stateString = "unknown";
	break;
    }

    DEBUG_PRINT(F("Door is "));
    DEBUG_PRINTLN(stateString);
    mqtt.publish(MQTT_DOOR_SENSOR_TOPIC, stateString, true);
}

Timers::HandlerResult Door::maintain() {
    State current{getState()};

    if (current != lastState) {
	auto currentTime{millis()};

	if (debounceStartTime == 0) {
	    debounceStartTime = currentTime;
	} else if (currentTime - debounceStartTime >= DOOR_SENSOR_DEBOUNCE_MSECS) {
	    publishState(current);
	    lastState = current;
	}
    } else {
	debounceStartTime = 0;
    }

    return Timers::TimerStatus::repeat;
}

void Door::handleControlMessage(const String& message) {
    if (message.equals("OPEN")) {
	DEBUG_PRINTLN(F("Opening door"));
	triggerOpener();
    } else if (message.equals("CLOSE")) {
	DEBUG_PRINTLN(F("Closing door"));
	triggerOpener();
    } else {
	DEBUG_PRINT(F("Unsupported command received: "));
	DEBUG_PRINTLN(message);
    }
}

void Door::triggerOpener() {
    setControl(HIGH);

    timers.in(DOOR_CONTROL_MSECS, [this]{ return setControl(LOW); });
}

Timers::HandlerResult Door::setControl(int state) {
    digitalWrite(control, state);

    return Timers::TimerStatus::completed;
}

};
