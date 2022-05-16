#include "client_data_request.h"

bool yeetnet::message::client_data_request::read(net_message& obj)
{
    return obj.read_string(client_version) && obj.read<DWORD64>(client_time);
}

yeetnet::net_message yeetnet::message::client_data_request::write()
{
    net_message obj;
    obj.write_str(client_version);
    obj.write<DWORD64>(client_time);
    return obj;
}
