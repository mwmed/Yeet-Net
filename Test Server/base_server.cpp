#include "base_server.h"
#include <core_handler.h>

void base_server::queue_handler()
{
	while (is_active) {
		auto queue = get_queue();
		bool first = true;
		while (!queue->empty()) {
			auto obj = queue->get_next(first);

			if (obj.is_valid()) {

				on_packet(obj);
			}

			first = false;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void base_server::client_check_activity_handler()
{
	while (is_active) {
		for (auto m_tree : _sessions) {
			auto m_session = m_tree.second;

			if (m_session && m_session->client) {
				auto client = m_session->client;
				if (client->ping_sync.is_timedout()) {
					spdlog::warn("Client timedout");
					m_session->disconnect();
				}
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void base_server::on_packet(yeetnet::queue_object& obj)
{

	obj.owner->mtx.lock();
	spdlog::info("Received Core {}", (std::int8_t)obj.packet.opcode);

	auto session = _sessions[obj.owner->get_id()];

	if (session) {
		yeetnet::core::on_handle_core_c2s(session, obj.packet.opcode, obj.packet.data);
	}
	else { spdlog::error("Null session!"); }
	obj.owner->mtx.unlock();


}
