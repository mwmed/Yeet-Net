#pragma once
#include "net_message.h"












namespace yeetnet {
	class yeetcrypt {
	public:
		inline yeetcrypt(){}


	public:
		net_message get_rsapubkey();
		bool generate_key();
		void generate_keys();

	public:
		net_message encrypt(net_message to_encrypt, bool& ret);
		net_message decrypt(net_message to_decrypt, bool& ret);

		net_message rsa_decrypt(net_message in, bool& ret);
		net_message rsa_encrypt(net_message to_encrypt);
		net_message encrypt_key(net_message to_encrypt, bool& ret);
	public:
		RSA* rsa;
		std::int8_t* aes_key = 0;
		std::int8_t* rc4_key = 0;

	private:
		size_t rsa_key_bits = 1024;
		EVP_PKEY_CTX* keyCtx;

		std::int16_t current_encrypt_counter = 0;
		std::int16_t current_decrypt_counter = 0;

		std::mutex mtx;
	};
}