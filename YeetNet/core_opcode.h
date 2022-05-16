#pragma once
#include "yeetnet_includes.h"

















namespace yeetnet {
	enum class core_opcode : std::int8_t {
		none,

		//S2C
		server_key_exchange,
		server_key_exchange_success,
		server_connection_success,
		server_protocol_error,

		//C2S
		client_key_exchange,
		client_data_request,

		//Common
		rmi,
		encrypted,



		opcode_max_value
	};

	enum class core_rmi : std::uint16_t {
		none = 0,


		client_ping = 65535
	};
}