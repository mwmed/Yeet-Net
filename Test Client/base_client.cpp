#include "base_client.h"
#include <core_handler.h>


bool base_client::start_connection(std::string m_targetip, std::uint16_t m_targetport)
{
	bool result = false;
	if (connect_to_server(m_targetip, m_targetport))
	{
		m_client = new yeetnet::net_client(this, client_socket, m_targetip, m_targetport);
		m_session = new session(m_client);
		m_session->net_version = client_version;
		m_client->begin_listening();

		result = true;
	}
	return result;
}

bool base_client::connect_to_server(std::string m_targetip, std::uint16_t m_targetport)
{
	bool result = false;
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == INVALID_SOCKET)
	{
		result = false;
	}
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_targetport);
	inet_pton(AF_INET, m_targetip.c_str(), &addr.sin_addr);
	int connResult = connect(client_socket, (sockaddr*)&addr, sizeof(addr));

	if (connResult != SOCKET_ERROR)
	{
		result = true;
	}


	return result;
}

void base_client::queue_handler()
{
	while (true) {
		auto queue = get_queue();
		bool first = true;
		while (!queue->empty()) {
			auto obj = queue->get_next(first);

			if (obj.is_valid()) {

				on_packet(obj);
			}

			first = false;
		}
	}
}

void base_client::on_packet(yeetnet::queue_object& obj)
{
	
	obj.owner->mtx.lock();
	spdlog::info("Received Opcode {}", (std::int8_t)obj.packet.opcode);

	yeetnet::core::on_handle_core_s2c(m_session, obj.packet.opcode, obj.packet.data);

	obj.owner->mtx.unlock();



}
