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

struct provider {
    provider(const char* name, Garaduino::Web::statusItems& items) : name(name), items(items) {};
    const char* name;
    Garaduino::Web::statusItems& items;
};

std::vector<provider> providers{};

void routeGetRoot(Request& req, Response& res) {
    DEBUG_PRINTLN(F("Web: routeGetRoot"));
    String host{req.get("Host")};
    res.set("Location", (host + "/index.html").c_str());
    res.sendStatus(301);
}

void routeGetIndex(Request& req, Response& res) {
    DEBUG_PRINTLN(F("Web: routeGetIndex"));
    P(pageHead) =
	"<!doctype html>\n"
	"<html lang=en>\n"
	"<head>\n"
	"<meta charset=utf-8>\n"
	"<link rel=\"stylesheet\" href=\"/styles.css\">\n"
	"<title>Garaduino</title>\n"
	"</head>\n"
	"<body>\n"
	"<div class=\"wrapper\">\n"
	"<header class=\"box header\">Garaduino</header>\n"
	"<div class=\"box status-items\">\n";
    P(pageFoot) =
	"</div>\n"
	"<footer class=\"box footer\">" GIT_VERSION "</footer>\n"
	"</div>\n"
	"</body>\n"
	"</html>\n";
    P(labelHead) =
	"<div class=\"box label\">\n";
    P(labelFoot) =
	"</div>\n";
    P(valueHead) =
	"<div class=\"box value\">\n";
    P(valueFoot) =
	"</div>\n";

    res.set("Content-Type", "text/html");
    res.printP(pageHead);
    for (auto& provider: providers) {
	DEBUG_PRINT(F("Web: status provider "));
	DEBUG_PRINT(provider.name);
	DEBUG_PRINT(F(" ..."));
	for (auto& item: provider.items) {
	    DEBUG_PRINT(F(" item: "));
	    DEBUG_PRINT(item.label);
	    DEBUG_PRINT(F(" ..."));
	    res.printP(labelHead);
	    res.println(item.label);
	    res.printP(labelFoot);
	    res.printP(valueHead);
	    res.println(item.value);
	    res.printP(valueFoot);
	}
	DEBUG_PRINTLN(F("done"));
    }
    res.printP(pageFoot);
}

void routeGetStyles(Request& req, Response& res) {
    DEBUG_PRINTLN(F("Web: routeGetStyles"));
    P(content) =
	"*, *:before, *:after { box-sizing: border-box; }\n"
	"body {\n"
	"margin: 40px;\n"
	"font-family: \"Open Sans\", \"sans-serif\";\n"
	"}\n"
	".wrapper {\n"
	"max-width: 940px;\n"
	"margin: 0 auto;\n"
	"display: grid;\n"
	"grid-gap: 10px;\n"
	"grid-template-columns: 1fr;\n"
	"}\n"
	".wrapper > * {\n"
	"width: auto;\n"
	"margin: 0;\n"
	"}\n"
	".header {\n"
	"text-align: center;\n"
	"font-size: 150%;\n"
	"}\n"
	".footer {\n"
	"text-align: right;\n"
	"font-size: 75%;\n"
	"}\n"
	".status-items {\n"
	"display: grid;\n"
	"grid-gap: 10px;\n"
	"grid-template-columns: 3fr 1fr;\n"
	"}\n"
	".status-items > * {\n"
	"width: auto;\n"
	"font-size: 125%;\n"
	"margin: 0;\n"
	"}\n"
	".value {\n"
	"text-align: center;\n"
	"}\n"
	".box {\n"
	"background-color: #444;\n"
	"color: #fff;\n"
	"border-radius: 5px;\n"
	"padding: 20px;\n"
	"}\n"
	".box .box {\n"
	"background-color: #ccc;\n"
	"color: #444;\n"
	"}\n";
    res.set("Content-Type", "text/css");
    res.printP(content);
}

};

namespace Garaduino {

void Web::start(TimerSet& timers) {
    DEBUG_PRINT(F("Web: initializing..."));

    providers.reserve(6);

    app.get("/", routeGetRoot);
    app.get("/index.html", routeGetIndex);
    app.get("/styles.css", routeGetStyles);

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

void Web::addStatusItems(const char* name, statusItems& items) {
    providers.emplace_back(name, items);
}

};
