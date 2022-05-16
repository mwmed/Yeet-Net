#pragma once
#include "net_config.h"

















namespace yeetnet {
	namespace message {
		class server_key_exchange {
		public:
			inline server_key_exchange(){}
			inline server_key_exchange(net_config m_config, net_message m_serverkey) : config(m_config), server_key(m_serverkey) {}


			net_config config;
			net_message server_key;



			bool read(net_message& obj);
			net_message write();
		};
	}
}