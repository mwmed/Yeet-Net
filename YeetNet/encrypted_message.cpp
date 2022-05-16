#include "encrypted_message.h"

bool yeetnet::message::encrypted_message::read(net_message& obj, bool decrypted)
{
    bool result = false;
    if (!decrypted) {
        if (obj.read<std::int8_t>((std::int8_t&)type)) {
            std::uint32_t message_length = 0;
            if (obj.read<std::uint32_t>(message_length)) {
                result = obj.read_bytes(encrypted_data, message_length);
            }
        }
    }
    else {
        if (obj.read<std::int8_t>((std::int8_t&)opcode)) {
            std::uint32_t message_length = 0;
            if (obj.read<std::uint32_t>(message_length)) {
                result = obj.read_bytes(message_data, message_length);
            }
        }
    }
    return result;
}

yeetnet::net_message yeetnet::message::encrypted_message::write()
{
    net_message obj;
    obj.write<std::int8_t>((std::int8_t)type);
    obj.write<std::uint32_t>(encrypted_data.get_length());
    obj.write_bytes(encrypted_data);

    return obj;
}
