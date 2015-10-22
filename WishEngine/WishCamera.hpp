#pragma once

#include "stdafx.h"

namespace Wish
{

	class wish_camera : public WishTransform
	{

	public:
		__Wish_Export wish_camera();
		__Wish_Export ~wish_camera();

		mat4 Projection;
		mat4 View;
		mat4 ViewProjection;

		__Wish_Export void Perspective(r32 fovY, r32 aspect, r32 zNear, r32 zFar);
		__Wish_Export void Ortho(r32 left, r32 right, r32 top, r32 bottom, r32 zNear, r32 zFar);
		__Wish_Export void CalculateViewMatrix();
		__Wish_Export void ApplyImmediate();
	};

};