#pragma once
#include "net_message.h"















namespace yeetnet {
	struct net_config {
		std::uint64_t message_max_length = 317440;
		double client_ping_delay = 60 * 16.6;



		net_message write();
		bool read(net_message& obj);
	};
}