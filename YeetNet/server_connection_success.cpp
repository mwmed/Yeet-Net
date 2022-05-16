#include "server_connection_success.h"

bool yeetnet::message::server_connection_success::read(net_message& obj)
{
    return obj.read<DWORD64>(server_time) && obj.read<uint64_t>(session_id);
}

yeetnet::net_message yeetnet::message::server_connection_success::write()
{
    net_message obj;
    obj.write<DWORD64>(server_time);
    obj.write<uint64_t>(session_id);
    return obj;
}
