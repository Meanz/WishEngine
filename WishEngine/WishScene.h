#pragma once

#include "WishTransform.h"
#include "WishGameObject.h"
#include "WishGeometry.h"
#include "WishCamera.h"
#include "WishLight.h"

namespace Wish
{

	struct wish_scene
	{
		wish_transform* Root;
		wish_camera Camera;

		void Init();
		void Process();
	};

	__Wish_Export wish_game_object* Wish_Scene_NewGameObject(const char* name);
	__Wish_Export wish_transform* Wish_Scene_NewTransform(const char* name);
	__Wish_Export wish_geometry* Wish_Scene_NewGeometry(const char* name);
	__Wish_Export wish_point_light* Wish_Scene_NewPointLight(const char* name);
	__Wish_Export wish_light* Wish_Scene_NewDirectionalLight(const char* name);

	__Wish_Export wish_transform* Wish_Scene_GetRoot();
	__Wish_Export wish_camera* Wish_Scene_GetCamera();
}