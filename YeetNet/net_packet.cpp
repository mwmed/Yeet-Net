#include "net_packet.h"

bool yeetnet::net_header::read_header(net_message& obj)
{
    return (obj.read<std::int16_t>(network_version) && obj.read<std::uint64_t>(payload_length));
}

yeetnet::net_message yeetnet::net_header::write_header()
{
    net_message obj;
    obj.write<std::int16_t>(network_version);
    obj.write<std::uint64_t>(payload_length);
    return obj;
}

bool yeetnet::net_packet::read_packet(net_message& obj)
{
    bool result = false;

    std::uint64_t len = 0;
    if (obj.read<std::int8_t>((std::int8_t&)opcode) && obj.read<std::uint64_t>(len)) {
        if (obj.read_bytes(data, len)) {
            result = true;
        }
    }


    return result;
}

yeetnet::net_message yeetnet::net_packet::write_packet()
{
    net_message obj;
    obj.write<std::int8_t>((std::int8_t)opcode);
    obj.write<std::uint64_t>(data.get_length());
    obj.write_bytes(data);
    return obj;
}

void yeetnet::net_packet::impl(core_opcode o, net_message d)
{
    data = d;
    opcode = o;
}
