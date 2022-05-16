#include "base_server.h"

#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32")

int main()
{
	srand((unsigned)time(NULL) * 125);

	SetConsoleTitleA("YeetServer");

	spdlog::info("Initializing WSA.");
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);


	base_server* server = new base_server(28019, "yeetnet.1.0");

	spdlog::info("Starting Server Listener.");
	std::thread m_serverlistener = std::thread([&]
		{
			if (server->create_socket()) {
				server->begin_listening();
			}
			else {
				spdlog::error("Failed to create server!");
			}
		});
	spdlog::info("Starting Queue Listener.");

	std::thread m_serverqueuethread = std::thread([&]
		{
			if (server->is_active) {
				server->queue_handler();
			}
			else {
				spdlog::error("Cant create queue thread cause of socket error.");
			}
		});

	spdlog::info("Starting ClientActivityCheckHandler.");
	std::thread m_serverclientactivity = std::thread([&] 
		{
			if (server->is_active) {
				server->client_check_activity_handler();
			}
			else {
				spdlog::error("Cant create client_activity_check_handler cause of socket error.");
			}
		});
	spdlog::info("Running!");
	m_serverlistener.join();
	m_serverqueuethread.join();
	m_serverclientactivity.join();

	spdlog::info("Server has stopped working, write anything to exit.");
	std::cin.get();





	WSACleanup();


	delete server;
	server = nullptr;
}

