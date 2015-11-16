namespace Wish
{

	wish_input::wish_input()
	{

	}

	wish_input::~wish_input()
	{

	}

	


	//Input
	b32 wish_input::IsKeyDown(u32 key)
	{
		wish_engine_context* context = Wish_Engine_GetContext();
		//Key bounds check
		if (key >= WISH_INPUT_MAX_KEYS)
			return 0;
		return  context->State->Input.KeysDown[key];
	}
	b32 wish_input::IsKeyPressed(u32 key)
	{
		wish_engine_context* context = Wish_Engine_GetContext();
		//Key bounds check
		if (key >= WISH_INPUT_MAX_KEYS)
			return 0;
		return  context->State->Input.KeysPressed[key];
	}
	b32 wish_input::IsKeyReleased(u32 key)
	{
		wish_engine_context* context = Wish_Engine_GetContext();
		//Key bounds check
		if (key >= WISH_INPUT_MAX_KEYS)
			return 0;
		return  context->State->Input.KeysReleased[key];
	}

	b32 wish_input::IsMouseDown(u32 mb)
	{
		wish_engine_context* context = Wish_Engine_GetContext();
		//Key bounds check
		if (mb >= WISH_INPUT_MAX_BUTTONS)
			return 0;
		return  context->State->Input.MouseButtonsDown[mb];
	}
	b32 wish_input::IsMousePressed(u32 mb)
	{
		wish_engine_context* context = Wish_Engine_GetContext();
		//Key bounds check
		if (mb >= WISH_INPUT_MAX_BUTTONS)
			return 0;
		return  context->State->Input.MouseButtonsPressed[mb];
	}

	b32 wish_input::IsMouseReleased(u32 mb)
	{
		wish_engine_context* context = Wish_Engine_GetContext();
		//Key bounds check
		if (mb >= WISH_INPUT_MAX_BUTTONS)
			return 0;
		return  context->State->Input.MouseButtonsReleased[mb];
	}

	i32 wish_input::GetMouseX()
	{
		wish_engine_context* context = Wish_Engine_GetContext();
		return context->State->Input.MouseX;
	}
	i32 wish_input::GetMouseY()
	{
		wish_engine_context* context = Wish_Engine_GetContext();
		return context->State->Input.MouseY;
	}

	i32 wish_input::GetMouseDX()
	{
		wish_engine_context* context = Wish_Engine_GetContext();
		return context->State->Input.MouseDX;
	}
	i32 wish_input::GetMouseDY()
	{
		wish_engine_context* context = Wish_Engine_GetContext();
		return context->State->Input.MouseDY;
	}
}