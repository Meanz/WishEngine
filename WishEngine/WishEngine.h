#pragma once

#include "WishPlatform.h"
#include "WishLua.h"

//These two interact
#include "WishRenderer.h"	//Renderer does not need things from scene?
#include "WishScene.h"		//Scene needs things from renderer

//
#include "WishUI.h"

//Asset System
#include "WishAssetManager.h"

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
		wish_asset_manager AssetManager;
		wish_scene Scene;
		wish_renderer Renderer;
		wish_ui UI;

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

	//This is scary stuff!?
	__Wish_Export wish_platform* Wish_Engine_GetPlatform();
	__Wish_Export wish_engine_context* Wish_Engine_GetContext();

	__Wish_Export i32 Wish_Window_GetWidth();
	__Wish_Export i32 Wish_Window_GetHeight();
	__Wish_Export r32 Wish_Window_GetAspect();
}