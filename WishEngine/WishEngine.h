#pragma once

namespace Wish
{

	typedef __Wish_Import void(*Wish_Imp_Void_Func)(void);
	struct wish_engine_callback
	{
		Wish_Imp_Void_Func OnInit;
		Wish_Imp_Void_Func OnUpdate;
		Wish_Imp_Void_Func OnFrame;
	};

	struct wish_engine_context
	{
		wish_game_state* State;
		wish_game_memory* Memory;

		wish_lua_state* LuaState;

		wish_engine_callback Callback;
		wish_assetmanager_context AssetManager;
		wish_scene_context Scene;
		wish_renderer_context Renderer;

		//Internal
		bool isRunning;
		bool isInitialized;

		//Temp
		i32 CurrentFPS;
	};

	//Mehehe
	i32 Wish_Engine_GetFPS();

	//Pipe for app
	__Wish_Export void Wish_Engine_SetApp(wish_engine_callback callback);

	//Input
	__Wish_Export b32 Wish_Input_IsKeyDown(u32 key);
	__Wish_Export b32 Wish_Input_IsKeyPressed(u32 key);
	__Wish_Export b32 Wish_Input_IsKeyReleased(u32 key);

	__Wish_Export i32 Wish_Input_GetMouseX();
	__Wish_Export i32 Wish_Input_GetMouseY();

	__Wish_Export i32 Wish_Input_GetMouseDX();
	__Wish_Export i32 Wish_Input_GetMouseDY();

	__Wish_Export b32 Wish_Input_IsMouseDown(u32 mb);
	__Wish_Export b32 Wish_Input_IsMousePressed(u32 mb);
	__Wish_Export b32 Wish_Input_IsMouseReleased(u32 mb);

	//This is scary stuff!?
	__Wish_Export wish_platform* Wish_Engine_GetPlatform();
	__Wish_Export wish_engine_context* Wish_Engine_GetContext();

	__Wish_Export i32 Wish_Window_GetWidth();
	__Wish_Export i32 Wish_Window_GetHeight();
	__Wish_Export r32 Wish_Window_GetAspect();
}