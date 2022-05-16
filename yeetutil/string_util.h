#pragma once
#include <iomanip>
#include <sstream>
#include <Windows.h>
















namespace yeetutil {
	std::string hex_str(BYTE* data, int len);
}