#pragma once
#include "net_config.h"

















namespace yeetnet {
	namespace message {
		class server_connection_success {
		public:
			inline server_connection_success() {}
			inline server_connection_success(DWORD64 st, std::uint64_t sid) : server_time(st), session_id(sid) {}


			DWORD64 server_time = 0;
			std::uint64_t session_id = 0;



			bool read(net_message& obj);
			net_message write();
		};
	}
}