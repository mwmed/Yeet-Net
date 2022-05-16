#pragma once
#include "net_session.h"


#include "server_key_exchange.h"
#include "client_key_exchange.h"
#include "encrypted_message.h"
#include "client_data_request.h"
#include "server_connection_success.h"
#include "rmi_message.h"










namespace yeetnet {
	namespace core {




		//S2C_HANDLERS -> used for acks(server -> client)
		void on_handle_core_c2s(net_session* m_session, core_opcode opcode, net_message smessage);
		void on_handle_serverkeyexchange(net_session* m_session, message::server_key_exchange smessage);
		void on_handle_serverconnectionsuccess(net_session* m_session, message::server_connection_success smessage);


		//C2S_HANDLERS -> used for reqs(client -> server)
		void on_handle_core_s2c(net_session* m_session, core_opcode opcode, net_message smessage);
		void on_handle_client_data_request(net_session* m_session, message::client_data_request smessage);

		void on_handle_clientkeyexchange(net_session* m_session, message::client_key_exchange smessage);



		//C2S_S2C_HANDLERS -> used for common packets (client -> server | server -> client)
		void on_handle_core_common(net_session* m_session, core_opcode opcode, net_message smessage);
		void on_handle_encrypted(net_session* m_session, message::encrypted_message smessage);
		void on_handle_rmi(net_session* m_session, message::rmi_message smessage);
	}
}