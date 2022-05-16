#include "client_key_exchange.h"

bool yeetnet::message::client_key_exchange::read(net_message& obj)
{
    bool result = false;
    uint16_t aes_key_length = 0, rc4_key_length = 0;
    if (obj.read<uint16_t>(aes_key_length) && obj.read_bytes(aes_key, aes_key_length)) {
        if (obj.read<uint16_t>(rc4_key_length) && obj.read_bytes(rc4_key, rc4_key_length)) {
            result = true;
        }
        else {
            spdlog::error("failed to read rc4key");
        }
    }
    else {
        spdlog::error("failed to read aeskey");
    }

    return result;
}

yeetnet::net_message yeetnet::message::client_key_exchange::write()
{
    net_message obj;
    obj.write<uint16_t>(aes_key.get_length());
    obj.write_bytes(aes_key);
    obj.write<uint16_t>(rc4_key.get_length());
    obj.write_bytes(rc4_key);
    return obj;
}
