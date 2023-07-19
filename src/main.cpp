#include <iostream>

// External libraries
#include <simdjson.h>
#include <libwebsockets.h>

//int main(int argc, char* argv[]) {
//	std::cout << "Hello libwebsockets and simdjson!" << std::endl;
//	return 0;
//}

int handle_event(void* user_data, const char* event, size_t len) {
	// Parse the event JSON using simdjson
	simdjson::dom::parser parser;
	simdjson::dom::element event_data = parser.parse(event, len);

	// Access the fields of the event data
	std::string field1 = event_data["field1"];
	int field2 = event_data["field2"];

	// Do something with the event data
	std::cout << "Received NOSTR event:\n";
	std::cout << "Field 1: " << field1 << "\n";
	std::cout << "Field 2: " << field2 << "\n";

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
	  { "nostr-event-protocol", handle_event, 0, 0 },
	  { NULL, NULL, 0, 0 } // Terminate the list
	};

	// Create the libwebsocket listen socket
	struct lws_vhost* vhost = lws_create_vhost(context, &protocols[0]);
	if (!vhost) {
		std::cerr << "Failed to create libwebsocket vhost\n";
		lws_context_destroy(context);
		return 1;
	}

	// Run the libwebsocket event loop
	while (true) {
		lws_service(context, /* timeout_ms = */ 50);
		// Add additional logic or break condition as needed
	}

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

	return 0;
}
