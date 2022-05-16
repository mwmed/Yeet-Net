#include "rmi_message.h"

bool yeetnet::message::rmi_message::read(net_message& obj)
{
    bool result = false;
    std::uint32_t length = 0;
    if (obj.read<uint16_t>(rmi_id) && obj.read<uint32_t>(length) && obj.read_bytes(message_data, length))
        result = true;

    return result;
}

yeetnet::net_message yeetnet::message::rmi_message::write()
{
    net_message obj;
    obj.write<uint16_t>(rmi_id);
    obj.write<uint32_t>(message_data.get_length());
    obj.write_bytes(message_data);
    return obj;
}
