#include <iostream>

// External libraries
#include <simdjson.h>
#include <libwebsockets.h>

std::string_view EXAMPLE_KEY = "43dfa3b174bc7a5e8145d12e393be34107ead39a1565518cd2a4691e8557352b";
std::string_view RELAY_URL = "";

struct lws* websocket;

std::string_view set_debug_url(int url_setting) {
	//For testing purposes and easy switching between servers
	switch (url_setting) {
	case 0:
		return "wss://relay.damus.io";
	case 1:
		return "ws://echo.websocket.events/.ws";
	case 2:
		return "wss://ws.postman-echo.com/raw";
	default:
		return "Invalid URL setting";
	}
}

void logCallback(int level, const char* line) {
	// Output or handle the log message as per your requirement
	std::cout << line << std::endl;
}

//lws_set_log_level(LLL_ERR | LLL_WARN | LLL_NOTICE | LLL_INFO | LLL_DEBUG | LLL_PARSER | LLL_HEADER | LLL_EXT | LLL_CLIENT, NULL);
//lws_set_log_cb(logCallback);

//////////////////////////////////////////////////////////////////////////
/* http-only */
struct per_session_data__http {
	HANDLE hFile;
};

static int callback_http(struct libwebsocket_context* context, struct libwebsocket* wsi, enum libwebsocket_callback_reasons reason, void* user, void* in, size_t len)
{
	int n, m;
	unsigned char* p;
	static unsigned char buffer[4096];

	TCHAR cache_file[MAX_PATH];
	//struct stat stat_buf;
	unsigned int fileSize = 0;
	struct per_session_data__http* pss =
		(struct per_session_data__http*)user;

	DWORD readLen = 0;

	char client_name[128];
	char client_ip[128];

	switch (reason) {
	case LWS_CALLBACK_HTTP:

		lwsl_notice("http.\n");
		/* check for the "send a big file by hand" example case */
		sprintf(cache_file, "%s\\cache.png", g_cache_path);
		p = buffer;

		pss->hFile = CreateFile(cache_file, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);

		if (pss->hFile == NULL || pss->hFile == INVALID_HANDLE_VALUE)
			return -1;
		/*
		 * we will send a big jpeg file, but it could be
		 * anything.  Set the Content-Type: appropriately
		 * so the browser knows what to do with it.
		 */

		p += sprintf((char*)p,
			"HTTP/1.0 200 OK\x0d\x0a"
			"Server: libwebsockets\x0d\x0a"
			"Content-Type: image/png\x0d\x0a"
			"Content-Length: %u\x0d\x0a\x0d\x0a",
			GetFileSize(pss->hFile, 0));

		/*
		 * send the http headers...
		 * this won't block since it's the first payload sent
		 * on the connection since it was established
		 * (too small for partial)
		 */
		n = libwebsocket_write(wsi, buffer,
			p - buffer, LWS_WRITE_HTTP);

		if (n < 0) {
			CloseHandle(pss->hFile);
			return -1;
		}
		/*
		 * book us a LWS_CALLBACK_HTTP_WRITEABLE callback
		 */
		libwebsocket_callback_on_writable(context, wsi);
		break;

	case LWS_CALLBACK_HTTP_FILE_COMPLETION:
		//		lwsl_info("LWS_CALLBACK_HTTP_FILE_COMPLETION seen\n");
				/* kill the connection after we sent one file */
		return -1;

	case LWS_CALLBACK_HTTP_WRITEABLE:
		/*
		 * we can send more of whatever it is we were sending
		 */
		do {
			if (ReadFile(pss->hFile, buffer, sizeof buffer, &readLen, NULL) == INVALID_HANDLE_VALUE)
			{
				CloseHandle(pss->hFile);
				return -1;
			}
			/* problem reading, close conn */
			if (readLen <= 0)
			{
				CloseHandle(pss->hFile);
				return -1;
			}
			/*
			 * because it's HTTP and not websocket, don't need to take
			 * care about pre and postamble
			 */
			m = libwebsocket_write(wsi, buffer, readLen, LWS_WRITE_HTTP);
			if (m < 0)
			{
				CloseHandle(pss->hFile);
				return -1;
			}
			if (m != readLen)
				/* partial write, adjust */
				SetFilePointer(pss->hFile, m - readLen, 0, FILE_CURRENT);

		} while (!lws_send_pipe_choked(wsi));
		libwebsocket_callback_on_writable(context, wsi);
		break;

		/*
		 * callback for confirming to continue with client IP appear in
		 * protocol 0 callback since no websocket protocol has been agreed
		 * yet.  You can just ignore this if you won't filter on client IP
		 * since the default uhandled callback return is 0 meaning let the
		 * connection continue.
		 */

	case LWS_CALLBACK_FILTER_NETWORK_CONNECTION:
		libwebsockets_get_peer_addresses(context, wsi, (int)(long)in, client_name, sizeof(client_name), client_ip, sizeof(client_ip));

		fprintf(stderr, "Received network connect from %s (%s)\n",
			client_name, client_ip);
		/* if we returned non-zero from here, we kill the connection */
		break;

	default:
		break;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
char pro_http[] = "http-only";
char pro_dumb_increment[] = "dumb-increment-protocol";
char pro_lws_command[] = "lws-command-protocol";
//////////////////////////////////////////////////////////////////////////
struct libwebsocket_protocols protocols[] = {
	{
		pro_http,			/* protocol name, char */
		callback_http,		/* callback function */
		sizeof(struct per_session_data__http),	/* per_session_data_size */
		0,	/* max buffer */
	},
	{ NULL, NULL, 0, 0 } /* terminator */
};
//////////////////////////////////////////////////////////////////////////


// Creating a websocket connection
void connecting_websocket(std::string_view server_url) {
	// Creating a websocket context
	struct lws_context_creation_info info;
	memset(&info, 0, sizeof(info));
	info.port = CONTEXT_PORT_NO_LISTEN;
	info.protocols = NULL;
	info.gid = -1;
	info.uid = -1;
	info.options = 0;
	info.user = NULL;
	info.ka_time = 0;
	info.ka_probes = 0;
	info.ka_interval = 0;

	// Creating a websocket context
	struct lws_context* context = lws_create_context(&info);
	if (context == NULL) {
		std::cout << "Creating context failed" << std::endl;
		return;
	}

	// Creating a websocket connection
	struct lws_client_connect_info ccinfo = { 0 };
	ccinfo.context = context;
	ccinfo.address = server_url.data();
	ccinfo.port = 443;
	ccinfo.path = "/";
	ccinfo.host = lws_canonical_hostname(context);
	ccinfo.origin = "origin";
	ccinfo.protocol = "protocol";
	ccinfo.ietf_version_or_minus_one = -1;
	ccinfo.userdata = NULL;
	ccinfo.ssl_connection = 0;


	// Creating a websocket connection
	websocket = lws_client_connect_via_info(&ccinfo);
	if (websocket == NULL) {
		std::cout << "Creating connection failed" << std::endl;
		return;
	}
}

int main(int argc, char* argv[]) {
	RELAY_URL = set_debug_url(0);
	std::cout << "Connecting to: " << RELAY_URL << std::endl;
	connecting_websocket(RELAY_URL);
    return 0;
}
