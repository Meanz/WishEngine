#include "stdafx.h"

#include "Wish.h"

#include "WishPlatform.cpp"
#include "WishEngine.cpp"
#include "WishMemory.cpp"
#include "WishLua.cpp"

namespace Wish {
	wish_game_memory* Wish_Get_GameMemory() {
		wish_engine_context* context = Wish_Engine_GetContext();
		return context->Memory;
	}
	wish_lua_state* Wish_Get_Lua() {
		wish_engine_context* context = Wish_Engine_GetContext();
		return context->LuaState;
	}
	wish_scene_context* Wish_Get_Scene() {
		wish_engine_context* context = Wish_Engine_GetContext();
		return &context->Scene;
	}
	wish_renderer_context* Wish_Get_Renderer() {
		wish_engine_context* context = Wish_Engine_GetContext();
		return &context->Renderer;
	}
};
__declspec(dllexport) void* operator new (size_t size, const char* filename, int line){
	void* ptr = new char[size];
	printf("NEW size = %i filename = %s line = %i \n", size, filename, line);
	return ptr;
}

__declspec(dllexport) void operator delete (void* data, size_t size, const char* filename, int line){
	delete data;
	printf("DELETE size = %i filename = %s line = %i \n", size, filename, line);
}