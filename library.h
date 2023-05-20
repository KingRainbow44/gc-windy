#pragma once

#include <cstdio>
#include <cstring>
#include <cstdint>
#include <vector>

#include "lua/lua.hpp"

extern "C" __declspec(dllexport) int __cdecl compile(const char* script, uint8_t* output);