#pragma once
#include "net_packet.h"
#include "net_client.h"






















namespace yeetnet {
	class queue_object
	{
	public:
		inline queue_object(){}
		inline queue_object(net_client* o, net_packet p) : owner(o), packet(p){};

		bool is_valid();
		net_client* owner = nullptr;
		net_packet packet;
	};
	class net_queue {
	public:
		inline net_queue(){}

		inline net_queue(std::uint64_t queue_id_p, std::string queue_tag_p) : queue_id(queue_id_p), queue_tag(queue_tag_p){};


	public:
		queue_object add_to_queue(net_client* owner, net_packet packet);

		queue_object get_next(bool first);

		bool empty();


	public:
		std::string get_tag();
		std::uint64_t get_id();

	private:
		std::deque<queue_object> objects;

		std::uint64_t queue_id = 0;
		std::string queue_tag = "RootQueue";
	};
}