#include "stdafx.h"
#include "Wish.hpp"
#include "WishGameObject.hpp"

namespace Wish
{
	//Top
	WishGameObject::WishGameObject(const char* name) : WishTransform(name)
	{
		TransformType = WISH_TRANSFORM_TYPE_GAME_OBJECT;
	}

	WishGameObject::~WishGameObject()
	{
	}
}