#pragma once
#include <Windows.h>
#include <iostream>












namespace yeetutil {
	std::uint32_t computehash(std::string Message);
	std::uint32_t computehash(BYTE* obj, size_t obj_length);

}