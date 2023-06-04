# USE P2Net instead
This is an example of using the Winsock library, which is considered bad practice. It is recommended to use the improved version of this project, P2Net, available at https://github.com/mwmed/p2net.


# Required Libraries
Openssl & spdlog

# Platform Support
Personally i only tested this project on windows, idk about the other OS.

# Server
The server can handle multiple clients at the same time and remove unused/dead clients automatically


# Handling User Packets
in base_client, base_server
```
virtual void on_rmi(yeetnet::net_session* m_session, yeetnet::message::rmi_message rmi) override {
	switch((blablarmi)rmi.rmi_id){
        case blablarmi::message_chat:
             on_handle_messagechar(rmi.message_data);
             break;
  }
}
