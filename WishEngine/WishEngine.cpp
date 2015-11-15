// WishEngine.cpp : Defines the entry point for the console application.
#define CONTEXT_CHECK ASSERT((context != NULL))

//Make this a pointer and let the state know about it?
//state->engine
//This way the platform layer has 100% control
namespace Wish
{
	wish_engine_context* context = NULL;

	i32 Wish_Engine_GetFPS()
	{
		CONTEXT_CHECK;
		return 0;
	}

	void Wish_Engine_SetApp(wish_engine_callback app)
	{
		CONTEXT_CHECK;
		context->Callback = app;
		if (context->Callback.OnInit) {
			context->Callback.OnInit();
		}
	}

	wish_platform* Wish_Engine_GetPlatform()
	{
		CONTEXT_CHECK;
		return &context->State->Platform;
	}

	wish_engine_context* Wish_Engine_GetContext()
	{
		CONTEXT_CHECK;
		return context;
	}

	i32 Wish_Window_GetWidth() {
		CONTEXT_CHECK;
		return context->State->Window.Width;
	}

	i32 Wish_Window_GetHeight() {
		CONTEXT_CHECK;
		return context->State->Window.Height;
	}

	r32 Wish_Window_GetAspect() {
		CONTEXT_CHECK;
		return context->State->Window.Aspect;
	}

}

void EngineInitialize(wish_game_state* state, wish_game_memory* memory)
{
	Wish_Memory_Init(memory);

	context = (wish_engine_context*)Wish_Memory_Alloc(memory, sizeof(wish_engine_context));
	Wish_Memory_Zero(context, sizeof(wish_engine_context));
	context->State = state;
	context->Memory = memory;

	//
	state->IsEngineInitialized = true;

	//Initialize our asset manager
	Wish_AssetManager_Init();

	//Initialize our renderer
	Wish_Renderer_Init(context->State->Window.Width, context->State->Window.Height);

	//Initialize our scene
	Wish_Scene_Init();

	//Initiaize our Lua state
	context->LuaState = Wish_Lua_NewState();
}

__Wish_Export _Def_Wish_Engine_OnInit(_Wish_Engine_OnInit)
{
	EngineInitialize(state, memory);
}


__Wish_Export _Def_Wish_Engine_OnReload(_Wish_Engine_OnReload)
{
	if (!state->IsEngineInitialized) {
		EngineInitialize(state, memory);
	}
	else
	{
		context = (wish_engine_context*)((u8*)memory->FirstBlock + (sizeof(wish_memory_block) * 2));
		context->State = state;
		context->Memory = memory;
	}
}

__Wish_Export _Def_Wish_Engine_OnFixedUpdate(_Wish_Engine_OnFixedUpdate)
{
	CONTEXT_CHECK
}

__Wish_Export _Def_Wish_Engine_OnUpdate(_Wish_Engine_OnUpdate)
{
	CONTEXT_CHECK

		if (Wish_Input_IsKeyReleased(wish_scancode::WISH_SCANCODE_5))
		{
			printf("Scancode 5\n");
			Wish_Lua_DoFile(context->LuaState, "./data/lua/test.lua");
		}
}

__Wish_Export _Def_Wish_Engine_OnFrame(_Wish_Engine_OnFrame)
{
	CONTEXT_CHECK
		if (context->Callback.OnFrame) {
			context->Callback.OnFrame();
		}

	//Resource processing
	Wish_AssetManager_Update();

	//Process everything
	Wish_Scene_Process();

	//Swap buffers :D
	//Flush the frame!
	Wish_Renderer_Flush();
}