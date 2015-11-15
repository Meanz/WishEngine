namespace Wish
{

	//Input
	b32 Wish_Input_IsKeyDown(u32 key)
	{
		wish_engine_context* context = Wish_Engine_GetContext();
		//Key bounds check
		if (key >= WISH_INPUT_MAX_KEYS)
			return 0;
		return  context->State->Input.KeysDown[key];
	}
	b32 Wish_Input_IsKeyPressed(u32 key)
	{
		wish_engine_context* context = Wish_Engine_GetContext();
		//Key bounds check
		if (key >= WISH_INPUT_MAX_KEYS)
			return 0;
		return  context->State->Input.KeysPressed[key];
	}
	b32 Wish_Input_IsKeyReleased(u32 key)
	{
		wish_engine_context* context = Wish_Engine_GetContext();
		//Key bounds check
		if (key >= WISH_INPUT_MAX_KEYS)
			return 0;
		return  context->State->Input.KeysReleased[key];
	}

	i32 Wish_Input_GetMouseX()
	{
		wish_engine_context* context = Wish_Engine_GetContext();
		return context->State->Input.MouseX;
	}
	i32 Wish_Input_GetMouseY()
	{
		wish_engine_context* context = Wish_Engine_GetContext();
		return context->State->Input.MouseY;
	}

	i32 Wish_Input_GetMouseDX()
	{
		wish_engine_context* context = Wish_Engine_GetContext();
		return context->State->Input.MouseDX;
	}
	i32 Wish_Input_GetMouseDY()
	{
		wish_engine_context* context = Wish_Engine_GetContext();
		return context->State->Input.MouseDY;
	}

	b32 Wish_Input_IsMouseDown(u32 mb)
	{
		wish_engine_context* context = Wish_Engine_GetContext();
		//Key bounds check
		if (mb >= WISH_INPUT_MAX_BUTTONS)
			return 0;
		return  context->State->Input.MouseButtonsDown[mb];
	}
	b32 Wish_Input_IsMousePressed(u32 mb)
	{
		wish_engine_context* context = Wish_Engine_GetContext();
		//Key bounds check
		if (mb >= WISH_INPUT_MAX_BUTTONS)
			return 0;
		return  context->State->Input.MouseButtonsPressed[mb];
	}

	b32 Wish_Input_IsMouseReleased(u32 mb)
	{
		wish_engine_context* context = Wish_Engine_GetContext();
		//Key bounds check
		if (mb >= WISH_INPUT_MAX_BUTTONS)
			return 0;
		return  context->State->Input.MouseButtonsReleased[mb];
	}
}