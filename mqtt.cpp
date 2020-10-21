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

#include "mqtt.hpp"

#include <array>
#include <algorithm>
#include <cstring>

namespace {

struct subscription {
    const char* topic;
    Garaduino::MQTT::subscriptionHandler handler;
};

using subscriptionArray = std::array<subscription, 8>;

subscriptionArray subscriptions{};

subscriptionArray::iterator findSlotForSubscription() {
    return std::find_if(subscriptions.begin(), subscriptions.end(), [](subscription& s) { return s.topic == nullptr; });
}

void callback(char* topic, byte* payload, unsigned int length) {
    payload[length] = '\0';

    DEBUG_PRINT(F("MQTT message - topic: "));
    DEBUG_PRINT(topic);
    DEBUG_PRINT(F(" payload: "));
    DEBUG_PRINTLN((char *)payload);

    for (auto& subscription: subscriptions) {
	if (!std::strcmp(subscription.topic, topic)) {
	    subscription.handler(String((char *)payload));
	}
    }
}

};

namespace Garaduino {

void MQTT::start() {
    DEBUG_PRINT(F("Initializing MQTT..."));

    mqtt.setServer(MQTT_BROKER_NAME, MQTT_BROKER_PORT);
    mqtt.setCallback(callback);

    if (connect()) {
	DEBUG_PRINTLN(F(" done"));
    } else {
	DEBUG_PRINTLN(F(" failed"));
    }

    timers.every(MQTT_POLL_SECS * 1000, [this]{ return maintain(); });
}

bool MQTT::connect() {
    if (mqtt.connect(MQTT_CLIENT_NAME, MQTT_STATUS_TOPIC, 0, true, "offline")) {
	mqtt.publish(MQTT_STATUS_TOPIC, "online", true);
	for (auto& message: queue) {
	    if (message.topic != nullptr) {
		mqtt.publish(message.topic, message.payload, message.retain);
		message.topic = nullptr;
	    }
	}
	for (auto& subscription: subscriptions) {
	    if (subscription.topic != nullptr) {
		mqtt.subscribe(subscription.topic);
	    }
	}
	return true;
    } else {
	return false;
    }
}

Timers::HandlerResult MQTT::maintain() {
    mqtt.loop();

    if (!mqtt.connected()) {
	DEBUG_PRINT(F("Connecting MQTT..."));
	if (connect()) {
	    DEBUG_PRINTLN(F(" done"));
	} else {
	    DEBUG_PRINTLN(F(" failed"));
	    // try again on the next cycle
	}
    }

    return Timers::TimerStatus::repeat;
}

MQTT::queuedMessageArray::iterator MQTT::findSlotForTopic(const char* topic) {
    if (auto result = std::find_if(queue.begin(), queue.end(), [topic](queuedMessage& m) { return m.topic == topic; }); result != queue.end()) {
	return result;
    }
    return std::find_if(queue.begin(), queue.end(), [](queuedMessage& m) { return m.topic == nullptr; });
}

bool MQTT::queueMessage(const char* topic, const char* payload, bool retain) {
    if (auto slot = findSlotForTopic(topic); slot != queue.end()) {
	slot->topic = topic;
	slot->payload = payload;
	slot->retain = retain;
	return true;
    } else {
	return false;
    }
}

bool MQTT::publish(const char* topic, const char* payload, bool retain) {
    if (mqtt.connected() && mqtt.publish(topic, payload, retain)) {
	return true;
    } else {
	return queueMessage(topic, payload, retain);
    }
}

bool MQTT::publish(const char* topic, const String& payload, bool retain) {
    return publish(topic, payload.c_str(), retain);
}

bool MQTT::subscribe(const char* topic, subscriptionHandler&& handler) {
    if (auto slot = findSlotForSubscription(); slot != subscriptions.end()) {
	slot->topic = topic;
	slot->handler = std::move(handler);
	return true;
    } else {
	return false;
    }
}

};
