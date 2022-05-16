#include "net_message.h"


void yeetnet::net_message::write_str(std::string obj, std::int8_t is_utf8)
{
    auto size = obj.size();
    write<std::uint8_t>(is_utf8);
    write<std::uint16_t>(is_utf8 == 1 ? size : size / 2);
    write_bytes((BYTE*)obj.c_str(), size);

}

void yeetnet::net_message::write_bytes(BYTE* obj, size_t obj_size)
{
    write_bytes(net_message(obj, obj_size));
}

void yeetnet::net_message::write_bytes(net_message obj)
{
    auto obj_length = obj.get_length();

    for (int i = 0; i < obj_length; i++)
        write<std::uint8_t>(obj.byte_array[i]);

}

bool yeetnet::net_message::read_string(std::string& obj)
{
    bool result = false;
    std::uint8_t string_type = 0;
    std::uint16_t string_length = 0;

    if (read<std::uint8_t>(string_type) && read<std::uint16_t>(string_length)) {
        net_message str_data;
        if (read_bytes(str_data, string_length)) {
            result = true;
            obj = std::string((char*)str_data.get_buffer(), string_length);
        }
    }

    return result;
}

bool yeetnet::net_message::read_bytes(net_message& obj, size_t read_off)
{
    bool result = true;
    std::int32_t offset = read_off;
    if (read_off == -1) {

        result = read<std::int32_t>(offset);
    }

    if (result && offset > 0) {
        BYTE* obj_buffer = new BYTE[offset];

        if (read_bytes(obj_buffer, offset)) {
            obj = net_message(obj_buffer, offset);
            result = true;
        }
        else {
            result = false;
        }
        delete[] obj_buffer;
    }

    return result;
}

bool yeetnet::net_message::read_bytes(BYTE* obj, size_t obj_length)
{
    bool result = false;
    if (read_offset + obj_length <= get_length()) {
        result = true;
        memcpy(obj, &byte_array[read_offset], obj_length);
        read_offset += obj_length;
    }

    return result;
}

size_t yeetnet::net_message::get_length()
{
    return byte_array.size();
}

BYTE* yeetnet::net_message::get_buffer()
{
    return byte_array.data();
}
