#include "stdafx.h"
#include "Wish.hpp"
#include "WishScene.hpp"

namespace Wish
{

	wish_game_object* Wish_Scene_NewGameObject(const char* name) {
		wish_game_object* wgo = (wish_game_object*)Wish_Memory_Alloc(sizeof(wish_game_object));
		Wish_Memory_Zero(wgo, sizeof(wish_game_object));
		Wish_Transform_Init(&wgo->Transform);
		wgo->Transform.TransformType = wish_transform_type::WISH_TRANSFORM_TYPE_GAME_OBJECT;
		Wish_Transform_SetName(&wgo->Transform, name);
		return wgo;
	}

	wish_transform* Wish_Scene_NewTransform(const char* name) {
		wish_transform* wt = (wish_transform*)malloc(sizeof(wish_transform));
		Wish_Memory_Zero(wt, sizeof(wish_transform));
		Wish_Transform_Init(wt);
		wt->TransformType = wish_transform_type::WISH_TRANSFORM_TYPE_TRANSFORM;
		Wish_Transform_SetName(wt, name);
		return wt;
	}

	wish_geometry* Wish_Scene_NewGeometry(const char* name) {
		wish_geometry* wtg = (wish_geometry*)Wish_Memory_Alloc(sizeof(wish_geometry));
		Wish_Memory_Zero(wtg, sizeof(wish_geometry));
		Wish_Transform_Init(&wtg->Transform);
		wtg->Transform.TransformType = wish_transform_type::WISH_TRANSFORM_TYPE_GEOMETRY;
		Wish_Transform_SetName(&wtg->Transform, name);
		return wtg;
	}

	wish_point_light* Wish_Scene_NewPointLight(const char* name) {
		wish_point_light* light = (wish_point_light*)malloc(sizeof(wish_point_light));
		Wish_Memory_Zero(light, sizeof(wish_point_light));
		Wish_Transform_Init(&light->base.Transform);
		light->base.m_LightType = LightType::LIGHT_POINT;
		light->base.Transform.TransformType = wish_transform_type::WISH_TRANSFORM_TYPE_LIGHT;
		Wish_Transform_SetName(&light->base.Transform, name);
		return light;
	}

	wish_light* Wish_Scene_NewDirectionalLight(const char* name) {
		wish_light* light = (wish_light*)malloc(sizeof(wish_light));
		Wish_Memory_Zero(light, sizeof(wish_light));
		Wish_Transform_Init(&light->Transform);
		light->m_LightType = LightType::LIGHT_DIRECTIONAL;
		light->Transform.TransformType = wish_transform_type::WISH_TRANSFORM_TYPE_LIGHT;
		Wish_Transform_SetName(&light->Transform, name);
		return light;
	}

}
void Wish::Wish_Scene_Init()
{
	wish_scene_context& scene = Wish_Engine_GetContext()->Scene;
	scene.root = Wish_Scene_NewTransform("Root");

	Wish_Memory_Zero(&scene.camera, sizeof(wish_camera));
	Wish_Transform_Init(&scene.camera.Transform);
	scene.camera.Projection = mat4(1.0f);
	scene.camera.View = mat4(1.0f);
	scene.camera.ViewProjection = mat4(1.0f);
}

wish_transform* Wish::Wish_Scene_GetRoot()
{
	wish_scene_context& scene = Wish_Engine_GetContext()->Scene;
	return scene.root;
}

wish_camera* Wish::Wish_Scene_GetCamera()
{
	wish_scene_context& scene = Wish_Engine_GetContext()->Scene;
	return &scene.camera;
}

int recurDepth = 0;
void ProcessNode(wish_transform* pTransform){

	wish_engine_context* ex = Wish_Engine_GetContext();
	int i = 0;
	if (pTransform->TransformType == wish_transform_type::WISH_TRANSFORM_TYPE_GEOMETRY) {

		//
		wish_geometry* geometry = (wish_geometry*)pTransform;

		//Push the geometry to the renderer
		Wish_Renderer_Submit(geometry);

	}
	else if (pTransform->TransformType == wish_transform_type::WISH_TRANSFORM_TYPE_LIGHT) {
		wish_light* light = (wish_light*)pTransform;
		Wish_Renderer_Submit(light);
	}
	else if (pTransform->TransformType == wish_transform_type::WISH_TRANSFORM_TYPE_GAME_OBJECT) {
		wish_game_object* gameObject = (wish_game_object*)pTransform;
		//Perform transform update
		//Perform fixed update
		if (gameObject->OnFixedUpdate != NULL) {
			gameObject->OnFixedUpdate();
		}
		//Update
		if (gameObject->OnUpdate != NULL)
		{
			gameObject->OnUpdate();
		}
		if (gameObject->cOnUpdate != NULL)
		{
			gameObject->cOnUpdate(gameObject->Obj);
		}

		//Draw
		if (gameObject->OnDraw != NULL)
		{
			gameObject->OnDraw();
		}

	}
	//Pass the rendering processing down to our children
	wish_transform* it = pTransform->Child;
	if (it)
	{
		//This will loop everything and break when it is parent
		while (it != NULL)
		{
			recurDepth++;
			ProcessNode(it);
			it = it->Next;
		}
	}
}

void Wish::Wish_Scene_Process()
{
	wish_scene_context& scene = Wish_Engine_GetContext()->Scene;

	//Unpack scene
	Wish_Renderer_SetCamera(&scene.camera);

	//Here is the genius part!
	//This doesn't do any rendering, but submits() whatever needs to be rendered, while at the same time
	//Doing the processing of each node
	//printf("root children: %s\n", std::to_string(scene.root->Children[0]->Children.size()).c_str());
	recurDepth = 0;
	ProcessNode(scene.root);
}