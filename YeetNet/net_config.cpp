#include "net_config.h"

yeetnet::net_message yeetnet::net_config::write()
{
    net_message obj;
    obj.write<std::uint64_t>(message_max_length);
    obj.write<double>(client_ping_delay);
    return obj;
}

bool yeetnet::net_config::read(net_message& obj)
{
    return (obj.read<std::uint64_t>(message_max_length) && obj.read<double>(client_ping_delay));
}
