#include "server_key_exchange.h"

bool yeetnet::message::server_key_exchange::read(net_message& obj)
{
    std::uint16_t key_length = 0;
    return (config.read(obj) && obj.read<std::uint16_t>(key_length) && obj.read_bytes(server_key, key_length));
}

yeetnet::net_message yeetnet::message::server_key_exchange::write()
{
    net_message obj;

    obj.write_bytes(config.write());
    obj.write<std::uint16_t>(server_key.get_length());
    obj.write_bytes(server_key);
    return obj;
}
