#pragma once
#include "net_client.h"
#include "yeetcrypt.h"
#include "encrypt_type.h"
#include "net_ping_sync.h"
#include "net_secure_user.h"

















namespace yeetnet {

	class net_session {
	public:
		inline net_session(){}

		inline net_session(net_client* client_p) : client(client_p) {}


	public:
		net_client* client = nullptr;
		yeetcrypt* crypt = nullptr;


	public:
		void send_encrypted(net_message obj, core::encrypt_type type);
		void send_encrypted(core_opcode opcode, net_message obj, core::encrypt_type type);
		void send_rmi(std::uint16_t rmi_id, net_message obj, core::encrypt_type type);
		void send_ping();

	public:
		net_secure_user secure_user;
		std::uint64_t id = 0;
		std::string net_version = "default";


	public:
		void disconnect();
	};
}