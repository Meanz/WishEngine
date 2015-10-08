#pragma once

#include <lua.hpp>

namespace Wish
{
	typedef lua_State wish_lua_state;

	wish_lua_state* Wish_Lua_NewState();

	void Wish_Lua_DoFile(wish_lua_state* L, const char* fn);
}