#pragma once
#include "net_message.h"

















namespace yeetnet {
	namespace message {
		class client_data_request {
		public:
			inline client_data_request() {}
			inline client_data_request(std::string cs, DWORD64 ct) : client_version(cs), client_time(ct) {}


			std::string client_version = "";
			DWORD64 client_time = 0;

			bool read(net_message& obj);
			net_message write();
		};
	}
}