#include <iostream>

// External libraries
#include <simdjson.h>
#include <libwebsockets.h>

std::string_view EXAMPLE_KEY = "43dfa3b174bc7a5e8145d12e393be34107ead39a1565518cd2a4691e8557352b";
std::string_view RELAY_URL = "";

struct lws* websocket;

std::string_view set_debug_url(int url_setting) {
	switch (url_setting) {
	case 0:
		return "wss://relay.damus.io";
	case 1:
		return "wws://echo.websocket.events/.ws";
	case 2:
		return "wss://ws.postman-echo.com/raw";
	default:
		return "Invalid URL setting";
	}
}

int main() {
	RELAY_URL = set_debug_url(0);
	return 0;
}