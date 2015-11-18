#pragma once

#include <lua.hpp>

namespace Wish
{
	typedef lua_State wish_lua_state;

	class wish_lua_script
	{
	public:
		wish_lua_state* L;

		wish_lua_script();
		~wish_lua_script();

		void DoFile(const char* fn);
	};
}