#pragma once

namespace Wish
{

	class WishGameObject : public WishTransform
	{

	public:
		__Wish_Export WishGameObject(const char* name);
		__Wish_Export ~WishGameObject();

		__Wish_Export virtual void OnUpdate() {};
		__Wish_Export virtual void OnFixedUpdate() {};
		__Wish_Export virtual void OnDraw() {};
	};
}