#include <iostream>
#include <string>

// External libraries
#include <simdjson.h>
#include <libwebsockets.h>

//int main(int argc, char* argv[]) {
//	std::cout << "Hello libwebsockets and simdjson!" << std::endl;
//	return 0;
//}

int nostr_event_parser(void* user_data, const char* event, size_t len) {
	// Parse the event JSON using simdjson
	simdjson::dom::parser parser;
	simdjson::dom::element event_data = parser.parse(event, len);

	// Access the fields of the event data
	std::string_view field1 = event_data["field1"];
	int field2 = event_data["field2"];

	// Do something with the event data
	std::cout << "Received NOSTR event:\n";
	std::cout << "Field 1: " << field1 << "\n";
	std::cout << "Field 2: " << field2 << "\n";

	return 0;
}

int handle_event(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, size_t len) {
	switch (reason) {
	case LWS_CALLBACK_ESTABLISHED:
		std::cout << "Connection established" << std::endl;
		break;
	case LWS_CALLBACK_RECEIVE:
		std::cout << "Received data: " << std::string((char*)in, len) << std::endl;
		break;
	case LWS_CALLBACK_CLOSED:
		std::cout << "Connection closed" << std::endl;
		break;
	default:
		break;
	}

	return 0;
}

int setup_websocket_context() {
	// Create the libwebsocket context
	struct lws_context_creation_info info;
	memset(&info, 0, sizeof(info));
	info.port = CONTEXT_PORT_NO_LISTEN;
	info.protocols = NULL;
	info.gid = -1;
	info.uid = -1;

	struct lws_context* context = lws_create_context(&info);
	if (!context) {
		std::cerr << "Failed to create libwebsocket context\n";
		return 1;
	}

	// Create the NOSTR event handling protocol
	struct lws_protocols protocols[] = {
		{
			// Name of the protocol
			"nostr",
			// Callback function for handling events
			handle_event,
			// Size of the per-session data
			0,
			// Maximum size of a message
			1024,
			// Number of connections that can wait for service
			0,
			// User data
			NULL,
			// Per-vhost user data
			0
		},
		// End of list
		{ NULL, NULL, 0, 0, 0, NULL, 0 }
	};

	// Create the libwebsocket listen socke
	struct lws_vhost* vhost = lws_create_vhost(context, &info);

	//struct lws_vhost* vhost = lws_create_vhost(context, &protocols[0]);
	//if (!vhost) {
	//	std::cerr << "Failed to create libwebsocket vhost\n";
	//	lws_context_destroy(context);
	//	return 1;
	//}

	// Run the libwebsocket event loop
	while (true) {
		std::cout << "Running event loop\n";
		lws_service(context, /* timeout_ms = */ 50);
		// Add additional logic or break condition as needed
		break;
	}
	std::cout << "Event loop finished\n";

	// Clean up resources
	lws_context_destroy(context);
	return 0;
}

int main() {
	// Set up the libwebsocket context and handle events
	if (setup_websocket_context() != 0) {
		std::cerr << "Failed to set up WebSocket context\n";
		return 1;
	}
	else {
		std::cout << "WebSocket context set up successfully\n";
	}

	int dummy = 0;
	std::cin >> dummy;
	return 0;
}
