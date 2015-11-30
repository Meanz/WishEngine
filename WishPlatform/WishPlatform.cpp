// WishPlatform.cpp : Defines the entry point for the console application.
//

#include "targetver.h"
#include <SDL.h>
#include <Wish.h>
#include "WishThreading.cpp"

#include <iostream>

using namespace std;

using namespace Wish;

struct wish_game_code
{
	//Pipes to the game
	void* Handle;
	Wish_Game_Entry* GameEntry;
};

struct wish_engine_code
{
	//Pipes to the engine
	void* Handle;

	Wish_Engine_OnInit* OnInit;
	Wish_Engine_OnReload* OnReload;
	Wish_Engine_OnFixedUpdate* OnFixedUpdate;
	Wish_Engine_OnFrame* OnFrame;
};

struct wish_sdl_window
{
	//#######
	//# SDL #
	//#######

	//The window reference
	SDL_Window* SDLWindow;
	//The OpenGL Context reference
	SDL_GLContext GLContext;
	//
	i32 Width;
	//
	i32 Height;
	//
	r32 Aspect;

	//For mouse grabbing
	i32 SaveGrabMouseX;
	i32 SaveGrabMouseY;

};

global_variable wish_sdl_window window;
global_variable b32 isMouseLocked;

__Wish_Export b32 IsMouseLocked()
{
	return isMouseLocked;
}

__Wish_Export void LockMouse()
{
	//SDL_WarpMouseInWindow(window.SDLWindow,
	//	window.Width / 2,
	//	window.Height / 2);
	//
	//SDL_SetWindowGrab(GetEngine()->GetWindow()->getSDL_Window(), SDL_TRUE);
	SDL_SetRelativeMouseMode(SDL_bool::SDL_TRUE);
	isMouseLocked = true;
}

__Wish_Export void FreeMouse()
{
	SDL_SetRelativeMouseMode(SDL_bool::SDL_FALSE);
	isMouseLocked = false;
}

bool Wish_SDL_Init_Window(wish_game_state* state, char* windowName, i32 width, i32 height)
{
	bool result = false;
	window.SDLWindow = SDL_CreateWindow(windowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		width, height,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	//If we couldn't create the window, exit
	if (window.SDLWindow != NULL)
	{

		//OpenGL 3.1 Core
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		window.GLContext = SDL_GL_CreateContext(window.SDLWindow);


		//If we couldn't create the GL context, destroy the window and exit
		if (window.GLContext != NULL)
		{
			if (SDL_GL_SetSwapInterval(1) == 0)
			{

				//Glew
				GLenum glewError = glewInit();
				//If we couldn't initialize GLew, destroy the window and exit
				if (glewError == GLEW_OK)
				{
					//Set member variables
					window.Width = width;
					window.Height = height;
					window.Aspect = ((r32)width / (r32)height);
					result = true;
				}
				else
				{
					printf("Could not create GL Context.\n");
					SDL_SetError("Unable to initialize Glew");
					SDL_DestroyWindow(window.SDLWindow);
					window.SDLWindow = NULL;
				}
			}
			else
			{
				printf("Could not set vsync.\n");
				SDL_SetError("Unable to set vsync");
				SDL_DestroyWindow(window.SDLWindow);
				window.SDLWindow = NULL;
			}

		}
		else
		{
			printf("Error %s\n", SDL_GetError());
			printf("Could not create window.\n");
			SDL_SetError("Unable to initialize OpenGL");
			SDL_DestroyWindow(window.SDLWindow);
			window.SDLWindow = NULL;
		}
	}
	return result;
}

bool Wish_SDL_Init(wish_game_state* state, char* windowName, i32 width, i32 height)
{
	bool result = false;
	bool sdlInit = SDL_Init(SDL_INIT_VIDEO) < 0 ? false : true;
	if (sdlInit)
	{
		//Start doing stuff
		if (Wish_SDL_Init_Window(state, "Wish Engine", width, height))
		{
			SDL_StartTextInput();
			state->IsInitialized = true;
			result = true;
		}
		else
		{
			printf("Could not create window.");
		}
	}
	else {
		printf("Could not initialize Wish engine.\n");
	}
	return result;
}

void LoadGameCode(wish_game_code* gameCode, char* file)
{
	void* ptr = SDL_LoadObject(file);
	if (ptr)
	{
		gameCode->Handle = ptr;

		//Lookup function
		gameCode->GameEntry = (Wish_Game_Entry*)SDL_LoadFunction(ptr, "_Wish_Game_Entry");
	}
	else
	{
		printf("Unable to load game code %s\n", file);
	}
}

void LoadEngineCode(wish_engine_code* engineCode, char* file)
{
	void* ptr = SDL_LoadObject(file);
	if (ptr)
	{
		engineCode->Handle = ptr;

		//Lookup function
		engineCode->OnInit = (Wish_Engine_OnInit*)SDL_LoadFunction(ptr, "_Wish_Engine_OnInit");
		engineCode->OnReload = (Wish_Engine_OnInit*)SDL_LoadFunction(ptr, "_Wish_Engine_OnReload");
		engineCode->OnFixedUpdate = (Wish_Engine_OnFixedUpdate*)SDL_LoadFunction(ptr, "_Wish_Engine_OnFixedUpdate");
		engineCode->OnFrame = (Wish_Engine_OnFrame*)SDL_LoadFunction(ptr, "_Wish_Engine_OnFrame");
	}
	else
	{
		printf("Unable to load engine code %s\n", file);
	}
}

int main(int argc, char* argv[])
{

	int w = 1024;
	int h = 768;
	char* title = "Wish Engine";

	wish_game_state state;
	SDL_zero(state);
	state.Window.Width = w;
	state.Window.Height = h;
	state.Window.Aspect = (r32)w / (r32)h;

	//Platform functions
	state.Platform.LockMouse = LockMouse;
	state.Platform.FreeMouse = FreeMouse;
	state.Platform.IsMouseLocked = IsMouseLocked;

	u32 toAlloc = Megabytes(512);
	wish_game_memory* memory = (wish_game_memory*)SDL_malloc(toAlloc);

	//Offset the perm memory to start + wish_game_memory
	memory->PermMemory = (void*)(((u8*)memory + sizeof(wish_game_memory)));
	memory->PermMemSize = toAlloc - sizeof(wish_game_memory);

	//Initialize stuff
	Wish_SDL_Init(&state, title, w, h);

	wish_engine_code engineCode;
	SDL_zero(engineCode);
	LoadEngineCode(&engineCode, "./Debug/WishEngine.dll");

	wish_game_code gameCode;
	SDL_zero(gameCode);
	LoadGameCode(&gameCode, "./Debug/LandsOfSonketo.dll");

	//The number of ticks per second
	const u32 TICKS_PER_SECOND = 30;
	//Number of ticks to skip until next tick
	const u32 SKIP_TICKS = 1000 / TICKS_PER_SECOND;
	//Max number of frames to skip if we are "lagging" before we run the next time
	const u32 MAX_FRAMESKIP = 10;

	//Interpolation value between this frame and last frame for smooth animation
	float interpolation;

	//Set running to true
	state.IsRunning = true;

	int fps = 0;
	u32 lastFpsCount = SDL_GetTicks();

	//Perform preload!
	//OnLoad();
	if (engineCode.Handle && engineCode.OnInit)
	{
		engineCode.OnInit(&state, memory);

		if (gameCode.Handle && gameCode.GameEntry)
		{
			gameCode.GameEntry(&state, memory);
		}
	}

	//GameLoop Related stuff
	u32 targetFrameRate = 59;
	r64 ticksPerFrame = 1000 / targetFrameRate;
	//The timestamp for the next tick
	u32 nextGameTick = SDL_GetTicks();
	u32 lastFrame = SDL_GetTicks();
	u32 frameStart, frameEnd, frameDelta;

	SDL_StartTextInput();
	b32 inputHasChanged = true;
	while (state.IsRunning) {
		//frameInfo.frameDelta = SDL_GetTicks() - frameInfo.frameTimestamp;
		//frameInfo.frameTimestamp = SDL_GetTicks();

		/*loops = 0;
		while (SDL_GetTicks() > nextGameTick && loops < MAX_FRAMESKIP) {
		if (engineCode.Handle && engineCode.OnFixedUpdate)
		{
		engineCode.OnFixedUpdate(&state, memory);
		}
		nextGameTick += SKIP_TICKS;
		loops++;
		}*/

		//
		//##### BEGIN INPUT #####
		//

		//Just to reduce work
		if (inputHasChanged)
		{
			SDL_zero(state.Input.KeysPressed);
			SDL_zero(state.Input.KeysReleased);
			SDL_zero(state.Input.MouseButtonsPressed);
			SDL_zero(state.Input.MouseButtonsReleased);
			inputHasChanged = false;
		}

		//SDL_PumpEvents();

		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			switch (e.type)
			{
			case SDL_QUIT:
				state.IsRunning = false;
				break;

			case SDL_TEXTINPUT:
				//handleKeys(e.text.text[0], x, y);

				//printf("Input: %c\n", e.text.text[0]);
				break;

			case SDL_KEYDOWN:
				if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
					state.IsRunning = false;
				}
				else {
					if (!state.Input.KeysDown[e.key.keysym.scancode]) {
						state.Input.KeysPressed[e.key.keysym.scancode] = true;
					}
					state.Input.KeysDown[e.key.keysym.scancode] = true;
				}
				inputHasChanged = true;
				break;

			case SDL_KEYUP:
				state.Input.KeysDown[e.key.keysym.scancode] = false;
				state.Input.KeysReleased[e.key.keysym.scancode] = true;
				inputHasChanged = true;

				if (e.key.keysym.scancode == SDL_SCANCODE_9)
				{
					//Well then
					SDL_UnloadObject(engineCode.Handle);
					printf("Unloading engine code! -- OnFrame path: %i\n", (i32*)engineCode.OnFrame);
					SDL_zero(engineCode);
					//I wonder what happened to the memory! =D
					int i = 0;
				}
				else if (e.key.keysym.scancode == SDL_SCANCODE_8)
				{
					if (engineCode.Handle == NULL)
					{
						LoadEngineCode(&engineCode, "WishEngine.dll");
						printf("Engine code reloaded! -- OnFrame path: %i\n", (i32*)engineCode.OnFrame);
						if (engineCode.Handle && engineCode.OnReload) {
							engineCode.OnReload(&state, memory);
						}
					}
					else
					{
						printf("Unload the engine code first\n");
					}
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (!state.Input.MouseButtonsDown[e.button.button]) {
					state.Input.MouseButtonsPressed[e.button.button] = true;
				}
				state.Input.MouseButtonsDown[e.button.button] = true;
				inputHasChanged = true;
				break;

			case SDL_MOUSEBUTTONUP:
				state.Input.MouseButtonsDown[e.button.button] = false;
				state.Input.MouseButtonsReleased[e.button.button] = true;
				inputHasChanged = true;
				break;

			default:
				break;
			}
		}


		//Look into this
		if (isMouseLocked)
		{
			SDL_GetRelativeMouseState(&state.Input.MouseX, &state.Input.MouseY);
			//Copy
			state.Input.MouseDX = state.Input.MouseX;
			state.Input.MouseDY = state.Input.MouseY;
		}
		else
		{
			i32 lastX = state.Input.MouseX;
			i32 lastY = state.Input.MouseY;
			SDL_GetMouseState(&state.Input.MouseX, &state.Input.MouseY);
			state.Input.MouseDX = state.Input.MouseX - lastX;
			state.Input.MouseDY = state.Input.MouseY - lastY;
		}

		//
		//##### END INPUT #####
		//

		//Frame limiter
		
		//if (SDL_GetTicks() - lastFrame > floor(ticksPerFrame))
		{
			frameStart = SDL_GetTicks();
			//Calculate interpolation
			interpolation = (SDL_GetTicks() - lastFrame) / (r32)ticksPerFrame;

			//Draw frame
			if (engineCode.Handle && engineCode.OnFrame)
			{
				engineCode.OnFrame(&state, memory);
			}

			//Increment fps count
			fps++;

			//If we have reached fps time
			if (SDL_GetTicks() - lastFpsCount > 1000) {
				//frameInfo.fps = fps;
				state.FPS = fps;
				fps = 0;
				lastFpsCount = SDL_GetTicks();
			}

			//Swap buffers
			SDL_GL_SwapWindow(window.SDLWindow);

			frameEnd = SDL_GetTicks();
			frameDelta = (frameEnd - frameStart) + 1;

			//Sleep
			if (frameDelta < floor(ticksPerFrame))
			{
				//Sleep((i32)floor(ticksPerFrame) - frameDelta);
			}
			else
			{
				//printf("Running behind, frame took %i ms\n", frameDelta);
			}
		}
	}

	SDL_StopTextInput();
	//Free SDL
	//PostQuitEvent
	//OnStop();
	return 0;
}

