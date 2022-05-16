#pragma once
#include "yeetnet_includes.h"














namespace yeetnet {
	class net_secure_user {
	public:
		inline net_secure_user(){}



		bool is_authenticated = false;
		bool is_key_exchanged = false;

		bool start_ping_thread = false;
	};
}