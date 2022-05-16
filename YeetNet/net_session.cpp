#include "net_session.h"
#include "encrypted_message.h"
#include "rmi_message.h"

#include <string_util.h>
void yeetnet::net_session::send_encrypted(net_message obj, core::encrypt_type type)
{
	if (crypt) {
		switch (type) {
		case core::encrypt_type::aes:
		{
			bool result = false;
			auto encryptedmessage = crypt->encrypt(obj, result);
			if (result) {
				spdlog::info("Sending Encrypted {}", yeetutil::hex_str(encryptedmessage.get_buffer(), encryptedmessage.get_length()));
				client->send_core(core_opcode::encrypted, message::encrypted_message(type, encryptedmessage).write());

			}
			else {
				spdlog::error("Failed to encrypt reliable");
			}
			break;
		}

		case core::encrypt_type::rsa:
		{
			break;
		}

		case core::encrypt_type::_xor:
		{
			break;
		}
		}
	}
	else {
		spdlog::info("Crypt is null");
	}
}

void yeetnet::net_session::send_encrypted(core_opcode opcode, net_message obj, core::encrypt_type type)
{
	net_message msg;
	msg.write<std::int8_t>((std::int8_t)opcode);
	msg.write<std::uint32_t>(obj.get_length());
	msg.write_bytes(obj);

	send_encrypted(msg, type);
}

void yeetnet::net_session::send_rmi(std::uint16_t rmi_id, net_message obj, core::encrypt_type type)
{
	message::rmi_message rmi(rmi_id, obj);

	auto msg = rmi.write();
	if (type != core::encrypt_type::none) {

		send_encrypted(core_opcode::rmi, msg, type);
	}
	else {
		client->send_core(core_opcode::rmi, msg);
	}
}

void yeetnet::net_session::send_ping()
{
	send_rmi((std::uint16_t)core_rmi::client_ping, net_message(), core::encrypt_type::aes);
}

void yeetnet::net_session::disconnect()
{
	client->disconnect(true);
}
