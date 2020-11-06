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

#include "ethernet.hpp"

#include "config.hpp"

#include <Ethernet.h>

namespace Garaduino {

void Ethernet::start(TimerSet& timers) {
    DEBUG_PRINT(F("Initializing Ethernet with DHCP..."));

    if (::Ethernet.begin(mac.data()) == 0) {
	DEBUG_PRINTLN(F(" failed"));
	if (::Ethernet.hardwareStatus() == EthernetNoHardware) {
	    DEBUG_PRINTLN(F("Ethernet hardware was not found."));
	}
	// no point in carrying on, so do nothing forevermore:
	while (true);
    }
    DEBUG_PRINT(F(" IPv4 address is "));
    DEBUG_PRINTLN(::Ethernet.localIP());

    timers.every(ETHERNET_POLL_SECS * 1000, [this]{ return maintain(); });
}

Timers::HandlerResult Ethernet::maintain() {
    switch (::Ethernet.maintain()) {
    case 1:
	//renewal failed
	DEBUG_PRINTLN(F("DHCP: renewal failed"));
	break;

    case 2:
	//renewal succeeded
	DEBUG_PRINTLN(F("DHCP: renewal succeeded"));
	DEBUG_PRINT(F("DHCP: IPv4 address is "));
	DEBUG_PRINTLN(::Ethernet.localIP());
	break;

    case 3:
	//rebinding failed
	DEBUG_PRINTLN(F("DHCP: rebinding failed"));
	break;

    case 4:
	//rebinding succeeded
	DEBUG_PRINTLN(F("DHCP: rebinding succeeded"));
	DEBUG_PRINT(F("DHCP: IPv4 address is "));
	DEBUG_PRINTLN(::Ethernet.localIP());
	break;

    default:
	//nothing happened
	break;
    }

    return Timers::TimerStatus::repeat;
}

};
