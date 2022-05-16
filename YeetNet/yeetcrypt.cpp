#include "yeetcrypt.h"

#include <crc32.h>
yeetnet::net_message yeetnet::yeetcrypt::get_rsapubkey()
{
    std::vector<uint8_t> der_pubkey;
    {
        der_pubkey.resize(140);
        uint8_t* keyPtr = der_pubkey.data();
        int keyLength;
        if (!(keyLength = i2d_RSAPublicKey(rsa, &keyPtr)))
        {
            return net_message();
        }
        der_pubkey.resize(keyLength);
    }


    return net_message(der_pubkey.data(), der_pubkey.size());
}

bool yeetnet::yeetcrypt::generate_key()
{
    rsa = RSA_new();
    if (!rsa)
        return false;

    BIGNUM* exponent = BN_new();
    if (!exponent)
    {
        RSA_free(rsa);
        return false;
    }

    BN_set_word(exponent, 65537);
    RSA_generate_key_ex(rsa, 1024, exponent, NULL);


    return true;
}

void yeetnet::yeetcrypt::generate_keys()
{
    aes_key = new std::int8_t[16];
    RAND_bytes((unsigned char*)aes_key, 16);

    rc4_key = new std::int8_t[16];
    RAND_bytes((unsigned char*)rc4_key, 16);
}

yeetnet::net_message yeetnet::yeetcrypt::encrypt(net_message to_encrypt, bool& ret)
{
    std::lock_guard<std::mutex> guard(mtx);
	ret = false;
	std::uint32_t padding = 16 - (to_encrypt.get_length() + 1 + 4 + 2) % 16;
	net_message EncryptStruct;
	net_message HashStruct;
	HashStruct.write<std::int16_t>(current_encrypt_counter);
	HashStruct.write_bytes(to_encrypt);
	EncryptStruct.write<std::int8_t>(padding);
	EncryptStruct.write<std::int32_t>(yeetutil::computehash(HashStruct.get_buffer(), HashStruct.get_length()));
	EncryptStruct.write_bytes(HashStruct);

	for (int i = 0; i < padding; i++)
		EncryptStruct.write<std::int8_t>(0);


	++current_encrypt_counter;
	size_t es = 0;
	EVP_CIPHER_CTX* ctx;
	ctx = EVP_CIPHER_CTX_new();
	if (ctx == NULL)
	{


		return net_message();

	}
	if (EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, (unsigned char*)aes_key, NULL) == 0)
	{

		return net_message();
	}
	EVP_CIPHER_CTX_set_padding(ctx, false);
	BYTE* out = new BYTE[EncryptStruct.get_length()];
	int outSize2 = 0;
	if (EVP_EncryptUpdate(ctx, out, &outSize2, EncryptStruct.get_buffer(), EncryptStruct.get_length()) == 0)
	{

		return net_message();
	}
	es += outSize2;
	outSize2 = 0;
	if (EVP_EncryptFinal_ex(ctx, EncryptStruct.get_buffer() + es, &outSize2) == 0)
	{

		return net_message();
	}
	es += outSize2;
	EVP_CIPHER_CTX_free(ctx);

	ret = true;
	net_message encrypted(out, es);
	delete[] out;
	return encrypted;
}

yeetnet::net_message yeetnet::yeetcrypt::decrypt(net_message to_decrypt, bool& ret)
{
	std::lock_guard<std::mutex> guard(mtx);

	net_message decrypted;
	EVP_CIPHER_CTX* ctx;
	ctx = EVP_CIPHER_CTX_new();
	ret = false;
	if (ctx == NULL)
	{

		return decrypted;
	}
	if (!EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), 0, (unsigned char*)aes_key, 0))
	{

		return decrypted;
	}
	EVP_CIPHER_CTX_set_padding(ctx, false);

	std::int8_t* arr = new std::int8_t[to_decrypt.get_length()];
	int ds = 0;
	size_t ds2 = 0;

	if (!EVP_DecryptUpdate(ctx, (unsigned char*)arr, &ds, to_decrypt.get_buffer(), to_decrypt.get_length()))
	{

		return decrypted;
	}
	ds2 += ds;
	ds = 0;
	if (!EVP_DecryptFinal_ex(ctx, (unsigned char*)(arr + ds2), &ds))
	{
		return decrypted;
	}
	ds2 += ds;
	EVP_CIPHER_CTX_free(ctx);
	ret = true;
	decrypted = net_message((BYTE*)arr, ds2);
	delete[] arr;
	return decrypted;
}

yeetnet::net_message yeetnet::yeetcrypt::rsa_decrypt(net_message in, bool& result)
{
	unsigned char* aes_key_decrypted = new unsigned char[16];
	int ret = RSA_private_decrypt(in.get_length(), in.get_buffer(), aes_key_decrypted, rsa, RSA_PKCS1_OAEP_PADDING);
	result = ret != -1;
	return net_message(aes_key_decrypted, 16);
}

yeetnet::net_message yeetnet::yeetcrypt::rsa_encrypt(net_message to_encrypt)
{
	size_t outsize = 0;
	BYTE* ret = new BYTE[RSA_size(rsa)];
	outsize = RSA_public_encrypt(to_encrypt.get_length(), (const unsigned char*)to_encrypt.get_buffer(), (unsigned char*)ret, rsa, RSA_PKCS1_OAEP_PADDING);
	net_message encrypted(ret, outsize);
	delete[] ret;
	return encrypted;
}

yeetnet::net_message yeetnet::yeetcrypt::encrypt_key(net_message to_encrypt,bool & ret)
{
	ret = false;
	size_t es = 0;
	EVP_CIPHER_CTX* ctx;
	ctx = EVP_CIPHER_CTX_new();
	if (ctx == NULL)
	{

		return net_message();

	}
	if (EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, (unsigned char*)aes_key, NULL) == 0)
	{
		return net_message();
	}
	EVP_CIPHER_CTX_set_padding(ctx, false);
	BYTE* out = new BYTE[to_encrypt.get_length()];
	int outSize2 = 0;
	if (EVP_EncryptUpdate(ctx, out, &outSize2, to_encrypt.get_buffer(), to_encrypt.get_length()) == 0)
	{
		return net_message();
	}
	es += outSize2;
	outSize2 = 0;
	if (EVP_EncryptFinal_ex(ctx, to_encrypt.get_buffer() + es, &outSize2) == 0)
	{
		return net_message();
	}
	es += outSize2;
	EVP_CIPHER_CTX_free(ctx);

	ret = true;
	net_message encrypted(out, es);
	delete[] out;
	return encrypted;
}
