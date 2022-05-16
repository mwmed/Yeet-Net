#include "net_client.h"
#include "net_server.h"
#include "net_packet.h"
#include "net_session.h"
static int header_length = 10;

void yeetnet::net_client::begin_listening()
{

	m_ListenerThread = std::thread([&] {
		ping_sync.mtx.lock();
		while (is_connected_f) 
		{
			try {
				BYTE* current_data = new BYTE[header_length];

				int received_bytes = recv(client, (char*)current_data, header_length, 0);

				if (received_bytes <= 0) {
					disconnect(true);
					break;
				}


				net_message packet_data(current_data, received_bytes);
				delete[] current_data;

				
				net_packet packet;
				if (!packet.read_header(packet_data)) {
					continue;
				}

				if (packet.payload_length >= server_cfg.message_max_length) {
					continue;
				}

				if (packet.payload_length > 0) {
					auto buffer_length = packet.payload_length;
					BYTE* packet_buffer = new BYTE[buffer_length];
					received_bytes = recv(client, (char*)packet_buffer, buffer_length, 0);

					if (received_bytes <= 0) {

						disconnect(true);
					    break;

					}

					net_message buffer(packet_buffer, buffer_length);
					if (!packet.read_packet(buffer)) {

						//invalid packet
						continue;
					}
					delete[] packet_buffer;
				}

		

				remote_server->packet_queue->add_to_queue(this, packet);

			}
			catch(std::exception ex){}

		}
		disconnect(false);
		});


}

void yeetnet::net_client::disconnect(bool continue_thread)
{
	try {

		is_connected_f = false;

		if (!continue_thread) {

			mtx.lock();

			if (m_ListenerThread.joinable())
				m_ListenerThread.detach();

			if (client)
			{
				closesocket(client);
				client = 0;
			}


			mtx.unlock();

			remote_server->on_disconnect(this);
		}

	}
	catch(std::exception ex){}
}

bool yeetnet::net_client::is_connected()
{
	return is_connected_f;
}

bool yeetnet::net_client::is_outdated_client(std::string version)
{
	return version != remote_server->net_version;
}



std::uint64_t yeetnet::net_client::get_id()
{
	return client_id;
}

void yeetnet::net_client::on_rmi(net_session* m_session, message::rmi_message smessage)
{
	remote_server->on_rmi(m_session, smessage);
}

void yeetnet::net_client::send_core(core_opcode opcode, net_message message)
{
	net_packet packet;

	packet.impl(opcode, message);

	send_packet(packet);
}

void yeetnet::net_client::send_packet(net_packet obj)
{
	net_message packet_buffer;
	obj.payload_length = obj.data.get_length() + 1 + 8;

	packet_buffer.write_bytes(obj.write_header());
	packet_buffer.write_bytes(obj.write_packet());

	send_data(packet_buffer);

}

void yeetnet::net_client::send_data(net_message obj)
{
	send(client, (char*)obj.get_buffer(), obj.get_length(),0);
}
