#pragma once

namespace Wish
{

	class wish_game_object : public wish_transform
	{

	public:
		__Wish_Export wish_game_object(const char* name);
		__Wish_Export ~wish_game_object();

		__Wish_Export virtual void OnUpdate() {};
		__Wish_Export virtual void OnFixedUpdate() {};
		__Wish_Export virtual void OnDraw() {};
	};
}