#pragma once
#include "yeetnet_includes.h"















namespace yeetnet {
	namespace core {
		enum class encrypt_type : int8_t {
			none,
			aes,
			rsa,
			_xor
		};
	}
}