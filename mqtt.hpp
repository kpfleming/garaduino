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

#include <array>
#include <functional>
#undef LITTLE_ENDIAN

#include <Ethernet.h>
#include <PubSubClient.h>

#include "garaduino.hpp"

namespace Garaduino {

class MQTT {
public:
    MQTT(TimerSet& timers) : timers(timers) {};
    ~MQTT() {};

    void start();

    bool publish(const char* topic, const char* payload, bool retain = false);
    bool publish(const char* topic, const String& payload, bool retain = false);

    using subscriptionHandler = std::function<void (const String&)>;
    bool subscribe(const char* topic, subscriptionHandler&& handler);

    // ensure that these objects will never be copied or moved
    // (this could only happen by accident)
    MQTT() = delete;
    MQTT(const MQTT&) = delete;
    MQTT& operator=(const MQTT&) = delete;
    MQTT(MQTT&&) = delete;
    MQTT& operator=(MQTT&&) = delete;

private:
    TimerSet& timers;

    EthernetClient ether{};
    PubSubClient mqtt{ether};

    bool connect();
    Timers::HandlerResult maintain();

    struct queuedMessage {
	const char* topic;
	const char* payload;
	bool retain;
    };
    using queuedMessageArray = std::array<queuedMessage, 16>;
    queuedMessageArray queue{};
    queuedMessageArray::iterator findSlotForTopic(const char* topic);
    bool queueMessage(const char* topic, const char* payload, bool retain);
};

};
