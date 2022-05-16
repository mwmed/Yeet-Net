#pragma once
#include <iostream>

#include <thread>
#include <mutex>

#include <vector>
#include <deque>
#include <map>


#include <WS2tcpip.h>


//lib
#include <spdlog/spdlog.h>

#include <openssl/rsa.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/pem.h>


#pragma comment(lib, "ws2_32")
#pragma comment(lib, "yeetutil.lib")