#include "net_queue.h"

yeetnet::queue_object yeetnet::net_queue::add_to_queue(net_client* owner, net_packet packet)
{
    auto obj = queue_object(owner,packet);

    objects.push_back(obj);

    return obj;
}

yeetnet::queue_object yeetnet::net_queue::get_next(bool first)
{
    if (!first) {
        objects.pop_front();
    }

    auto obj = objects.size() > 0 ? objects.front() : queue_object();
    return obj;
}

bool yeetnet::net_queue::empty()
{
    return objects.empty();
}

std::string yeetnet::net_queue::get_tag()
{
    return queue_tag;
}

std::uint64_t yeetnet::net_queue::get_id()
{
    return queue_id;
}

bool yeetnet::queue_object::is_valid()
{
    return (owner && owner->is_connected() && packet.network_version == 0x7413);
}
