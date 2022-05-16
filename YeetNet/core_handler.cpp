#include "core_handler.h"
#include <string_util.h>

void yeetnet::core::on_handle_core_s2c(net_session* m_session, core_opcode opcode, net_message smessage)
{
	switch (opcode) {
	case yeetnet::core_opcode::server_key_exchange:
	{
		yeetnet::message::server_key_exchange serverkeyexchange;
		if (serverkeyexchange.read(smessage)) {
			yeetnet::core::on_handle_serverkeyexchange(m_session, serverkeyexchange);
		}
		break;
	}
	case yeetnet::core_opcode::server_key_exchange_success:
	{
		m_session->secure_user.is_key_exchanged = true;
		m_session->send_encrypted(core_opcode::client_data_request, message::client_data_request(m_session->net_version, GetTickCount64()).write(), yeetnet::core::encrypt_type::aes);
		break;
	}

	default:
		on_handle_core_common(m_session,opcode, smessage);
		break;
	}
}

void yeetnet::core::on_handle_client_data_request(net_session* m_session, message::client_data_request smessage)
{
	if (!m_session->secure_user.is_authenticated) {

		m_session->secure_user.is_authenticated = true;

		// Initialize ping sync
		m_session->client->ping_sync.first_client_time = smessage.client_time;
		m_session->client->ping_sync.on_update();

		if (m_session->client->is_outdated_client(smessage.client_version)) {
			spdlog::info("Outdated client client={}", smessage.client_version);
			m_session->disconnect();
		}
		else {
			m_session->send_encrypted(core_opcode::server_connection_success, message::server_connection_success(GetTickCount64(), m_session->id).write(), core::encrypt_type::aes);

		}
	}
}

void yeetnet::core::on_handle_serverkeyexchange(net_session* m_session, message::server_key_exchange smessage)
{

	if (!m_session->crypt) {
		spdlog::info("Received ServerKeyExchange! MaxPacketLength {} PingTime {}", smessage.config.message_max_length, smessage.config.client_ping_delay);

		m_session->client->ping_sync.interval = smessage.config.client_ping_delay;
		auto buffer = (std::int8_t*)smessage.server_key.get_buffer();
		auto crypt = new yeetcrypt();
		crypt->rsa = d2i_RSAPublicKey(0, (const unsigned char**)&buffer, smessage.server_key.get_length());
		crypt->generate_keys();

		bool fastkeyenc_res = false;
		auto encrypted_fastkey = crypt->encrypt_key(net_message((BYTE*)crypt->rc4_key, 16), fastkeyenc_res);

		if (fastkeyenc_res) {
			auto encrypted_aeskey = crypt->rsa_encrypt(net_message((BYTE*)crypt->aes_key, 16));

			message::client_key_exchange clientkeyexchange(encrypted_aeskey, encrypted_fastkey);

			m_session->client->send_core(core_opcode::client_key_exchange, clientkeyexchange.write());

			m_session->crypt = crypt;
		}
		else {
		}
	}
	else {
	}
}

void yeetnet::core::on_handle_serverconnectionsuccess(net_session* m_session, message::server_connection_success smessage)
{
	m_session->client->ping_sync.first_client_time = smessage.server_time;
	m_session->secure_user.start_ping_thread = true;

	m_session->client->ping_sync.first_client_time = smessage.server_time;
	m_session->client->ping_sync.on_update();

	m_session->client->ping_sync.mtx.unlock();




}

void yeetnet::core::on_handle_core_c2s(net_session* m_session, core_opcode opcode, net_message smessage)
{
	switch (opcode) {
	case yeetnet::core_opcode::client_key_exchange: {

		yeetnet::message::client_key_exchange clientkeyexchange;
		if (clientkeyexchange.read(smessage)) {
			yeetnet::core::on_handle_clientkeyexchange(m_session, clientkeyexchange);
		}
		break;
	}

	default:
		on_handle_core_common(m_session,opcode,smessage);
		break;
	}
}

void yeetnet::core::on_handle_clientkeyexchange(net_session* m_session, message::client_key_exchange message)
{
	if (!m_session->secure_user.is_key_exchanged && m_session->crypt && message.aes_key.get_length() > 0 && message.rc4_key.get_length() > 0) {

		auto aesbuffer = message.aes_key.get_buffer();
		auto rc4buffer = message.rc4_key.get_buffer();


		auto crypt = m_session->crypt;

		bool res = false;
		auto decrypted_aes = crypt->rsa_decrypt(message.aes_key, res);

		if (res) {
			crypt->aes_key = new std::int8_t[16];

			memcpy(crypt->aes_key, decrypted_aes.get_buffer(), 16);

			//ToDo add RC4 key decryption


			m_session->client->send_core(core_opcode::server_key_exchange_success, net_message());
			m_session->secure_user.is_key_exchanged = true;

		}
		else {
			m_session->client->send_core(core_opcode::server_protocol_error, net_message());
			m_session->disconnect();

			spdlog::info("KeyExchange failed!");
		}


	}
}

void yeetnet::core::on_handle_core_common(net_session* m_session, core_opcode opcode, net_message smessage)
{

	switch (opcode) {
	case core_opcode::encrypted:
	{
		message::encrypted_message encryptedmessage;
		if (encryptedmessage.read(smessage)) {
			on_handle_encrypted(m_session, encryptedmessage);
		}
		else spdlog::error("failed to read encrypted message");

		break;
	}
	case core_opcode::rmi:
	{
		message::rmi_message rmimessage;
		if (rmimessage.read(smessage)) {
			on_handle_rmi(m_session, rmimessage);
		}
		break;
	}
	case core_opcode::client_data_request:
	{
		message::client_data_request clientdatarequest;
		spdlog::info("Message {}", yeetutil::hex_str(smessage.get_buffer(), smessage.get_length()));

		if (clientdatarequest.read(smessage)) {
			on_handle_client_data_request(m_session, clientdatarequest);
		}
		else spdlog::error("failed to read data request");
		break;
	}
	case core_opcode::server_connection_success:
	{
		message::server_connection_success serverconnectionsuccess;
		if (serverconnectionsuccess.read(smessage)) {
			on_handle_serverconnectionsuccess(m_session, serverconnectionsuccess);
		}
		break;
	}
	}
}

void yeetnet::core::on_handle_encrypted(net_session* m_session, message::encrypted_message smessage)
{
	if (m_session->secure_user.is_key_exchanged && m_session->crypt) {

		switch (smessage.type) 
		{
		case encrypt_type::aes:
		{
			bool result = false;
			auto decrypted = m_session->crypt->decrypt(smessage.encrypted_data, result);
			if (result) {
				std::int8_t padding = 0;
				std::int32_t crc32 = 0;
				std::int16_t encrypt_counter = 0;

				if (decrypted.read(padding) && decrypted.read(crc32) && decrypted.read(encrypt_counter)) {

					if (smessage.read(decrypted, true)) {
						on_handle_core_common(m_session, smessage.opcode, smessage.message_data);
					}
					else spdlog::error("failed to read decrypted message");
				}
			}
			break;
		}
		case encrypt_type::rsa:
		case encrypt_type::_xor:
			break; //ToDo

		}
	}

}

void yeetnet::core::on_handle_rmi(net_session* m_session, message::rmi_message smessage)
{
	if (!m_session->secure_user.is_authenticated)
		return;

	spdlog::info("Received RMI={}", smessage.rmi_id);
	switch ((core_rmi)smessage.rmi_id) {
	case core_rmi::client_ping:
	{
		m_session->client->ping_sync.on_update();
		spdlog::info("Client Ping={}", m_session->client->ping_sync.ping);
		break;
	}
	default:
		m_session->client->on_rmi(m_session, smessage);
		break;
	}
}
