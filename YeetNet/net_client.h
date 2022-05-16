#pragma once

#include "net_packet.h"
#include "net_config.h"
#include "net_ping_sync.h"
#include "rmi_message.h"















namespace yeetnet {
	class net_server;
	class net_session;
	class net_client {
	public:
		inline net_client() {}

		inline net_client(net_server* server, SOCKET client_socket, std::string ip, uint64_t gen_id) : remote_server(server), client(client_socket), ipaddr(ip), client_id(gen_id) {}


		void begin_listening();
		void disconnect(bool continue_thread = false);
		bool is_connected();
		bool is_outdated_client(std::string version);
		std::uint64_t get_id();

	public:
		void on_rmi(net_session* m_session, message::rmi_message smessage);

	public:
		void send_core(core_opcode opcode, net_message message);
		void send_packet(net_packet obj);
		void send_data(net_message obj);
	public:
		net_ping_sync ping_sync;
		std::mutex mtx;

	private:
		std::thread m_ListenerThread;
		net_server* remote_server = nullptr;

		std::string ipaddr = "127.0.0.1";
		SOCKET client = 0;
		uint64_t client_id = 0;
		std::uint16_t remote_port = 0;

		net_config server_cfg;
		bool is_connected_f = true;
	};
}