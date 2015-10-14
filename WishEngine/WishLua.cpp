#include "stdafx.h"

#include "WishLua.h"
#include "Wish.hpp"

namespace Wish
{

#define lua_func static int

	/*
	To refer to elements in the stack, the API uses indices.
	The first element in the stack (that is, the element that was pushed first) has index 1, the next one has index 2, and so on. 
	We can also access elements using the top of the stack as our reference, 
	using negative indices. In that case, -1 refers to the element at the top (that is, the last element pushed), -2 to the previous element, and so on. 
	For instance, the call lua_tostring(L, -1) returns the value at the top of the stack as a string. As we will see,
	there are several occasions when it is natural to index the stack from the bottom (that is, with positive indices) 
	and several other occasions when the natural way is to use negative indices.*/
	// 5   4   3    2   1
	//[1, 25, 19, 111, 55] 0, 1, 3, 4
	// -1  -2  -3  -4  -5
	//LIFO - Last In First Out 

	lua_func l_sin(wish_lua_state* L)
	{
		double d = luaL_checknumber(L, 1); /* Get argument */
		lua_pushnumber(L, sin(d)); /* Push result */
		return 1; /* Number of arguments */
	}


	lua_func Wish_Lua_Scene_NewTransform(wish_lua_state* L)
	{

		//?
		const char* name = luaL_checkstring(L, 1);

		//Send this to the scenegraph
		//Wish_Scene_NewTransform(name);
		printf("Name: %s . \n", name);

		//Figure out how to return a representation of this thing
		lua_pushnumber(L, 5);

		return 1; /* Number of arguments */
	}


	static const struct luaL_reg SceneLib[] = {
		{"NewTransform", Wish_Lua_Scene_NewTransform},
		{ NULL, NULL } /* Sentinel */
	};


	static void stackDump(lua_State *L) {
		int i;
		int top = lua_gettop(L);
		for (i = 1; i <= top; i++) {  /* repeat for each level */
			int t = lua_type(L, i);
			switch (t) {

			case LUA_TSTRING:  /* strings */
				printf("`%s'", lua_tostring(L, i));
				break;

			case LUA_TBOOLEAN:  /* booleans */
				printf(lua_toboolean(L, i) ? "true" : "false");
				break;

			case LUA_TNUMBER:  /* numbers */
				printf("%g", lua_tonumber(L, i));
				break;

			default:  /* other values */
				printf("%s", lua_typename(L, t));
				break;

			}
			printf("  ");  /* put a separator */
		}
		printf("\n");  /* end the listing */
	}

	//

	wish_lua_state* Wish_Lua_NewState()
	{
		//init state
		lua_State* L = luaL_newstate();

		luaJIT_setmode(L, 0, LUAJIT_MODE_ENGINE | LUAJIT_MODE_ON);

		//load libraries
		luaL_openlibs(L);

		//Open my lib
		luaL_openlib(L, "Scene", SceneLib, 0);

		//do file

		lua_pushcfunction(L, l_sin);
		lua_setglobal(L, "mysin");

		return L;
	}

	void Wish_Lua_DoFile(wish_lua_state* L, const char* fn)
	{
		//Grab state
		if (luaL_dofile(L, fn))
		{
			printf("Lua encountered an error.\n");

			//Pop offset 1 from the stack
			//lua_pop(L, 1);

			//Get that thing 
			const char* error = lua_tostring(L, -1);
			if (error)
			{
				printf("LUA: %s\n", error);
			}
			else
			{
				printf("Even lua can't tell us what's wrong.");
			}
		}
		else
		{
			printf("Lua said everything went fine.\n");
		}

		stackDump(L);
	}
}