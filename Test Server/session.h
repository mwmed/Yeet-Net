#pragma once
#include <net_session.h>




















class session : public yeetnet::net_session {
public:

	session(yeetnet::net_client* m_client) : yeetnet::net_session(m_client) 
	{
	}

};