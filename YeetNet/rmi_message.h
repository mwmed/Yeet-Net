#pragma once
#include "net_message.h"




















namespace yeetnet {

	namespace message {
		class rmi_message {
		public:
			inline rmi_message() {}
			inline rmi_message(std::uint16_t rmid, net_message md) : rmi_id(rmid) , message_data(md){}



			std::uint16_t rmi_id = 0;
			net_message message_data;


			bool read(net_message& obj);

			net_message write();


		};
	}
}