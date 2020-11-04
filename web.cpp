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

#include "web.hpp"

#include "gitversion.hpp"

#include "config.hpp"

namespace {

void webHandlerIndex(Request& req, Response& res) {

    P(page) =
	"<html>\n"
	"<head>\n"
	"<title>Garaduino - Version: " GIT_VERSION "</title>\n"
	"</head>\n"
	"<body>\n"
	"<h1>Greetings middle earth!</h1>\n"
	"</body>\n"
	"</html>";

    res.set("Content-Type", "text/html");
    res.printP(page);
}

};

namespace Garaduino {

void Web::start() {
    DEBUG_PRINT(F("Initializing Web..."));

    app.get("/", webHandlerIndex);

    DEBUG_PRINTLN(F(" done"));

    timers.every(WEB_POLL_SECS * 1000, [this]{ return maintain(); });
}

Timers::HandlerResult Web::maintain() {
    EthernetClient client = server.available();
    if (client.connected()) {
	app.process(&client);
	client.stop();
    }

    return Timers::TimerStatus::repeat;
}

};
