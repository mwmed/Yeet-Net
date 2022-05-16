#pragma once
#include "net_client.h"
#include "net_queue.h"
#include "net_session.h"
#include "rmi_message.h"













namespace yeetnet {
	class net_server {
	public:
		inline net_server(){}
		inline net_server(std::uint16_t m_sock, std::string ver = "yeetnet.1.0") : server_port(m_sock) , net_version(ver) {}

	public:
		bool create_socket();
		void begin_listening();

	public:
		double get_elapsed_time();
		inline SOCKET get_socket();


		net_queue* get_queue();

	public:
		bool is_active = true;
		std::string net_version = "default";


	private:
		friend class net_client;
		net_queue* packet_queue = new net_queue();
	private:
		SOCKET server_socket = 0;
		std::uint16_t server_port = 0;
		double start_time = 0;


	protected:
		friend class net_client;
		virtual bool on_connect(net_server* server, SOCKET client, std::string ipaddr, std::uint16_t remote_port) {
			return true;
		}

		virtual void on_disconnect(net_client* client) {

		}

		virtual void on_rmi(net_session* m_session, message::rmi_message rmi) {

		}
	};
}