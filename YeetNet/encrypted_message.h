#pragma once
#include "encrypt_type.h"
#include "core_opcode.h"
#include "net_message.h"




















namespace yeetnet {

	namespace message {
		class encrypted_message {
		public:
			inline encrypted_message(){}

			inline encrypted_message(core::encrypt_type t, net_message ed) : type(t), encrypted_data(ed){}


			core::encrypt_type type = core::encrypt_type::none;
			
			//Encrypted -> read before decryption routine
			net_message encrypted_data;

			//Decrypted -> read after decryption routine ( base struct )
			core_opcode opcode = core_opcode::none;
			net_message message_data;


			bool read(net_message& obj, bool decrypted = false);

			net_message write();


		};
	}
}