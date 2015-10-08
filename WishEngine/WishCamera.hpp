#pragma once

#include "stdafx.h"

namespace Wish
{

	struct wish_camera
	{
		wish_transform Transform;
		mat4 Projection;
		mat4 View;
		mat4 ViewProjection;
	};

	__Wish_Export void Wish_Camera_Perspective(wish_camera* camera, float fovY, float aspect, float zNear, float zFar);
	__Wish_Export void Wish_Camera_Ortho2D(wish_camera* camera, float left, float right, float top, float bottom, float zNear, float zFar);

	//void LookAt(vec3 eye, vec3 target, vec3 up);
	__Wish_Export void Wish_Camera_CalculateViewMatrix(wish_camera* camera);

	//Apply this camera as immediate
	__Wish_Export void Wish_Camera_ApplyImmediate(wish_camera* camera);

	__Wish_Export void Wish_Camera_LookAt(wish_camera* camera, vec3 eye, vec3 center);


};