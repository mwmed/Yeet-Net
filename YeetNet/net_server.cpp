#include "net_server.h"

bool yeetnet::net_server::create_socket()
{
	bool result = true;
	try {
		server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		SOCKADDR_IN sock_addr;
		sock_addr.sin_port = htons(server_port);
		sock_addr.sin_family = AF_INET;
		sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		if (bind(server_socket, (SOCKADDR*)&sock_addr, sizeof(sock_addr)) == SOCKET_ERROR)
		{
			result = false;
		}
		if (listen(server_socket, 10) == SOCKET_ERROR)
		{
			result = false;
		}
	}
	catch (std::exception ex) {
		result = false;
	}

	is_active = result;
	return result;
}

void yeetnet::net_server::begin_listening()
{
	try {
		while (is_active) {
			struct sockaddr name;
			int nameLen = sizeof(name);
			SOCKET client = 0;
			client = accept(server_socket, &name, &nameLen);
			struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&name;
			struct in_addr ipAddr = pV4Addr->sin_addr;
			char str[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &ipAddr, str, INET_ADDRSTRLEN);

			if (client != SOCKET_ERROR)
			{
				std::string ip = std::string(str, strlen(str));
				on_connect(this, client, ip, htons(pV4Addr->sin_port));

			}
		}
	}
	catch (std::exception ex) {

	}
}

double yeetnet::net_server::get_elapsed_time()
{
    return GetTickCount64() - start_time;
}

inline SOCKET yeetnet::net_server::get_socket()
{
    return server_socket;
}

yeetnet::net_queue* yeetnet::net_server::get_queue()
{
	return packet_queue;
}
