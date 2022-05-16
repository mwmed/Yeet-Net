#include "net_ping_sync.h"

void yeetnet::net_ping_sync::on_update()
{
	ping = get_current_ping() / 1000;
	auto tick = GetTickCount64();
	if (first_client_time == 0)
		first_client_time = tick;
	last_sent_time = tick;
	last_received_time = tick;
}

double yeetnet::net_ping_sync::get_elapsed_time()
{
	return (GetTickCount64() - last_sent_time);
}

double yeetnet::net_ping_sync::get_current_ping()
{
	return (get_elapsed_time() - interval);
}

bool yeetnet::net_ping_sync::should_send_ping()
{
	return get_current_ping() >= interval;
}

bool yeetnet::net_ping_sync::is_timedout()
{
	return get_current_ping() - buffer_delay >= interval;
}
