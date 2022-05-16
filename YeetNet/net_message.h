#pragma once
#include "yeetnet_includes.h"


















namespace yeetnet {
	class net_message {
	public:
		inline net_message(){}
		inline net_message(BYTE* obj, size_t obj_size) 
		{ 
			for (int i = 0; i < obj_size; i++) 
				byte_array.push_back(obj[i]); 

			read_offset = 0;
			write_offset = 0;
		}



		template<class T>
		inline void write(T obj) {
			std::uint8_t* bytes = (std::uint8_t*)&obj;

			auto obj_size = sizeof(T);
			for (int i = 0; i < obj_size; i++)
				byte_array.push_back(bytes[i]);

			write_offset += obj_size;
		}
		template<class T>
		inline bool read(T& obj) {
			auto buffer_size = byte_array.size();
			auto obj_size = sizeof(T);
			bool result = false;

			if (read_offset + obj_size <= buffer_size) {
				result = true;

				memcpy(&obj, &byte_array[read_offset], obj_size);

				read_offset += obj_size;
			}

			return result;
		}

		void write_str(std::string obj, std::int8_t is_utf8 = 1);
		void write_bytes(BYTE* obj, size_t obj_size);
		void write_bytes(net_message obj);


		bool read_string(std::string& obj);
		bool read_bytes(net_message& obj, size_t read_off = -1);
		bool read_bytes(BYTE* obj, size_t obj_length);

	public:
		size_t get_length();
		BYTE* get_buffer();


	private:
		std::vector<std::uint8_t> byte_array;
		std::uint64_t write_offset = 0;
		std::uint64_t read_offset = 0;
	};
}