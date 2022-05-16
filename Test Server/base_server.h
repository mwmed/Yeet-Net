#pragma once
#include <net_server.h>
#include <server_key_exchange.h>
#include <yeetcrypt.h>

#include "session.h"






#pragma comment(lib, "yeetnet.lib")








class base_server : public yeetnet::net_server {
public:

	base_server(std::uint16_t m_port, std::string ver = "yeetnet.1.0") : yeetnet::net_server(m_port, ver) {
		m_servercrypt = new yeetnet::yeetcrypt();
		m_servercrypt->generate_key();
	}


	void queue_handler();
	void client_check_activity_handler();

public:
	yeetnet::yeetcrypt* m_servercrypt = nullptr;

protected:
	virtual bool on_connect(net_server* server, SOCKET client, std::string ipaddr, std::uint16_t remote_port) override {
		try {
			++next_client_id;

			auto base_client = new yeetnet::net_client(server, client, ipaddr, next_client_id);

			auto base_session = new session(base_client);
			base_session->crypt = new yeetnet::yeetcrypt();

			base_session->crypt->rsa = m_servercrypt->rsa;

			base_session->client->begin_listening();

			

			base_client->send_core(yeetnet::core_opcode::server_key_exchange, yeetnet::message::server_key_exchange(yeetnet::net_config(), m_servercrypt->get_rsapubkey()).write());


			_sessions[next_client_id] = base_session;

		}
		catch (std::exception ex) {

		}
		return true;
	}
	virtual void on_disconnect(yeetnet::net_client* client) override {
		auto client_id = client->get_id();


		if (_sessions.find(client_id) != _sessions.end()) {

			auto _session = _sessions[client_id];

			if (_session) {
				delete _session->client;
				_session->client = nullptr;

				delete _session;
				_session = nullptr;
			}
		}
	}
	virtual void on_rmi(yeetnet::net_session* m_session, yeetnet::message::rmi_message rmi) override {
	
	}

private:
	void on_packet(yeetnet::queue_object& obj);
private:
	std::map<std::uint64_t, session*> _sessions;

	std::uint64_t next_client_id = 0;

	std::thread m_packetthread;
};