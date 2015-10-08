#pragma once

//To get the naming's correctly
#include <WishCore.h>

namespace Wish
{

	//fwd decls
	struct wish_camera;
	void Wish_Camera_LookAt(wish_camera*, v3, v3);
	void Wish_Camera_CalculateViewMatrix(wish_camera*);
	void Wish_Camera_ApplyImmediate(wish_camera*);

	struct wish_fps_camera
	{
		wish_camera* Camera;
		r32 Yaw;
		r32 Pitch;
		v3 Position;
	};

	v3 Wish_Math_DirectionFromEuler(r32 yaw, r32 pitch)
	{
		//Calculate the direction vector
		r32 yawRads = glm::radians(yaw);
		r32 pitchRads = glm::radians(pitch);
		vec3 dir = vec3(
			cos(pitchRads) * cos(yawRads),
			sin(pitchRads),
			cos(pitchRads) * sin(yawRads)
			);
		dir = glm::normalize(dir);
		return dir;
	}

	inline v3 Wish_FPSCamera_GetDirection(wish_fps_camera* camera)
	{
		return Wish_Math_DirectionFromEuler(camera->Yaw, camera->Pitch);
	}

	v3 Wish_FPSCamera_GetLeftStrifeDirection(wish_fps_camera* camera)
	{
		v3 dir = Wish_Math_DirectionFromEuler(camera->Yaw, camera->Pitch);
		vec3 dir_l = glm::cross(vec3(0.0f, 1.0f, 0.0f), dir);
		dir_l = glm::normalize(dir_l);
		return dir_l;
	}

	void Wish_FPSCamera_MoveForward(wish_fps_camera* camera, r32 units)
	{
		v3 dir = Wish_FPSCamera_GetDirection(camera);
		camera->Position += dir * units;
	}

	//- units = left, + units = right
	void Wish_FPSCamera_Strafe(wish_fps_camera* camera, r32 units)
	{
		v3 dir = Wish_FPSCamera_GetLeftStrifeDirection(camera);
		camera->Position += dir * units;
	}

	void Wish_FPSCamera_Update(wish_fps_camera* camera)
	{
		Wish_Camera_LookAt(camera->Camera, camera->Position, camera->Position + (Wish_FPSCamera_GetDirection(camera) * 2.0f));
		Wish_Camera_CalculateViewMatrix(camera->Camera);
		Wish_Camera_ApplyImmediate(camera->Camera);
	}
}