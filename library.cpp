#include "library.h"

extern "C" __declspec(dllexport) int __cdecl compile(const char* script, uint8_t* output)
{
    // Create a new Lua state.
    auto* state = luaL_newstate();
    // Open libraries for the state.
    luaL_openlibs(state);

    // Load the script into the state.
    if (luaL_loadstring(state, script) != LUA_OK) {
        // If the script failed to load, print the error.
        printf("Error: %s\n", lua_tostring(state, -1));
        // Close the state.
        lua_close(state);
        // Return a null pointer.
        return -1;
    }

    // Compile the script into bytecode.
    std::vector<unsigned char> compiled;
    if (lua_dump(state, [](lua_State* _, const void* data, const size_t size, void* user) {
        auto* part = static_cast<const uint8_t*>(data);
        auto* buffer = static_cast<std::vector<uint8_t>*>(user);
        buffer->insert(buffer->end(), part, part + size);
        return 0;
    }, &compiled, 1) != LUA_OK) {
        // If the script failed to compile, print the error.
        printf("Error: %s\n", lua_tostring(state, -1));
        // Close the state.
        lua_close(state);
        // Return a null pointer.
        return -1;
    }

    // Close the state.
    lua_close(state);
    // Copy the compiled state to a new buffer.
    const auto size = compiled.size();
    memcpy(output, compiled.data(), compiled.size());
    
    // Return the buffer.
    return size;
}
