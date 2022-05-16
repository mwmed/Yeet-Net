#pragma once
#include "net_message.h"

















namespace yeetnet {
	namespace message {
		class client_key_exchange {
		public:
			inline client_key_exchange() {}
			inline client_key_exchange(net_message ak, net_message rk) : aes_key(ak), rc4_key(rk){}


			net_message aes_key;
			net_message rc4_key;

			bool read(net_message& obj);
			net_message write();
		};
	}
}