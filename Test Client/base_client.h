#pragma once
#include <net_server.h>
#include "session.h"

#include <WS2tcpip.h>

#pragma comment(lib, "yeetnet.lib")
#pragma comment(lib, "ws2_32.lib")








class base_client : public yeetnet::net_server {
public:
	inline base_client(){}


public:
	bool start_connection(std::string m_targetip, std::uint16_t m_targetport);
	bool connect_to_server(std::string m_targetip, std::uint16_t m_targetport);

public:
	void queue_handler();
	void on_packet(yeetnet::queue_object& obj);

	session* get_session() {
		return m_session;
	}

private:
	SOCKET client_socket = 0;
	std::string client_version = "yeetnet.1.0";

private:
	yeetnet::net_client* m_client = nullptr;
	session* m_session = nullptr;


protected:
	virtual bool on_connect(net_server* server, SOCKET client, std::string ipaddr, std::uint16_t remote_port) override {
		return true;
	}

	virtual void on_disconnect(yeetnet::net_client* client) override {

	}
};