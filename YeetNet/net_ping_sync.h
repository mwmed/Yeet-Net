#pragma once
#include "yeetnet_includes.h"











namespace yeetnet {
	class net_ping_sync {
	public:
		inline net_ping_sync() {}

		double interval = 0.0;
		DWORD64 first_client_time = 0;

		DWORD64 last_sent_time = 0;
		DWORD64 last_received_time = 0;

		double ping = 0;

	public:
		void on_update();
		double get_elapsed_time();
		double get_current_ping();
		bool should_send_ping();
		bool is_timedout();

	public:
		std::mutex mtx;
	private:
		double buffer_delay = 5250;
	};
}