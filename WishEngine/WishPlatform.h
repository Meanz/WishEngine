#pragma once

using namespace Wish;

struct wish_platform
{
	Wish_Bool_Func IsMouseLocked;
	Wish_Void_Func LockMouse;
	Wish_Void_Func FreeMouse;
};

#define WISH_INPUT_MAX_KEYS 512
#define WISH_INPUT_MAX_BUTTONS 20

struct wish_input
{
	b32 KeysDown[WISH_INPUT_MAX_KEYS];

	b32 KeysPressed[WISH_INPUT_MAX_KEYS];
	b32 KeysReleased[WISH_INPUT_MAX_KEYS];

	b32 MouseButtonsDown[WISH_INPUT_MAX_BUTTONS];
	b32 MouseButtonsPressed[WISH_INPUT_MAX_BUTTONS];
	b32 MouseButtonsReleased[WISH_INPUT_MAX_BUTTONS];

	i32 MouseX;
	i32 MouseY;
	i32 MouseDX;
	i32 MouseDY;
};

struct wish_window
{
	i32 Width;
	i32 Height;
	r32 Aspect;
};

struct wish_game_state
{
	wish_input Input;
	wish_window Window;
	wish_platform Platform;

	b32 IsEngineInitialized;
	b32 IsInitialized;
	b32 IsRunning;
};

extern "C"
{
	//Frame Event
#define _Def_Wish_Game_Entry(name) void name(wish_game_state* state, wish_game_memory* memory)
	typedef _Def_Wish_Game_Entry(Wish_Game_Entry);
	__Wish_Export _Def_Wish_Game_Entry(_Wish_Game_Entry);
#define Wish_GameEntry() __Wish_Export _Def_Wish_Game_Entry(_Wish_Game_Entry)

	//Reload Event
#define _Def_Wish_Engine_OnReload(name) void name(wish_game_state* state, wish_game_memory* memory)
	typedef _Def_Wish_Engine_OnReload(Wish_Engine_OnReload);
	__Wish_Export _Def_Wish_Engine_OnReload(_Wish_Engine_OnReload);

	//Init Event
#define _Def_Wish_Engine_OnInit(name) void name(wish_game_state* state, wish_game_memory* memory)
	typedef _Def_Wish_Engine_OnInit(Wish_Engine_OnInit);
	__Wish_Export _Def_Wish_Engine_OnInit(_Wish_Engine_OnInit);

	//Fixed Update Event
#define _Def_Wish_Engine_OnFixedUpdate(name) void name(wish_game_state* state, wish_game_memory* memory)
	typedef _Def_Wish_Engine_OnFixedUpdate(Wish_Engine_OnFixedUpdate);
	__Wish_Export _Def_Wish_Engine_OnFixedUpdate(_Wish_Engine_OnFixedUpdate);

	//Update event
#define _Def_Wish_Engine_OnUpdate(name) void name(wish_game_state* state, wish_game_memory* memory)
	typedef _Def_Wish_Engine_OnUpdate(Wish_Engine_OnUpdate);
	__Wish_Export _Def_Wish_Engine_OnUpdate(_Wish_Engine_OnUpdate);

	//Frame Event
#define _Def_Wish_Engine_OnFrame(name) void name(wish_game_state* state, wish_game_memory* memory)
	typedef _Def_Wish_Engine_OnFrame(Wish_Engine_OnFrame);
	__Wish_Export _Def_Wish_Engine_OnFrame(_Wish_Engine_OnFrame);

};