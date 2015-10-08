#pragma once

#include "WishTransform.hpp"
#include "WishGameObject.hpp"
#include "WishCamera.hpp"
#include "WishLight.hpp"

namespace Wish
{

	struct wish_scene_context
	{
		wish_transform* root;
		wish_camera camera;
	};

	__Wish_Export wish_game_object* Wish_Scene_NewGameObject(const char* name);
	__Wish_Export wish_transform* Wish_Scene_NewTransform(const char* name);
	__Wish_Export wish_geometry* Wish_Scene_NewGeometry(const char* name);
	__Wish_Export wish_point_light* Wish_Scene_NewPointLight(const char* name);
	__Wish_Export wish_light* Wish_Scene_NewDirectionalLight(const char* name);

	__Wish_Export wish_transform* Wish_Scene_GetRoot();
	__Wish_Export wish_camera* Wish_Scene_GetCamera();
	
	__Wish_Export void Wish_Scene_Init();
	__Wish_Export void Wish_Scene_Add(wish_transform* transform);
	__Wish_Export void Wish_Scene_Process();
}