#pragma once
#include "net_message.h"
#include "core_opcode.h"



















namespace yeetnet {
	class net_header {
	public:
		std::int16_t network_version = 0x7413;
		std::uint64_t payload_length = 0;


		bool read_header(net_message& obj);
		net_message write_header();
	};

	class net_packet : public net_header {
	public:
		net_message data;
		core_opcode opcode = core_opcode::none;


		bool read_packet(net_message& obj);
		net_message write_packet();

		void impl(core_opcode o, net_message d);
	};
}