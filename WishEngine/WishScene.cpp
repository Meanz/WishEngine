#include "stdafx.h"
#include "Wish.h"
#include "WishCore.h"

#include "WishTransform.cpp"
#include "WishGameObject.cpp"
#include "WishGeometry.cpp"
#include "WishLight.cpp"
#include "WishCamera.cpp"

namespace Wish
{

	wish_game_object* Wish_Scene_NewGameObject(const char* name) {
		void* mem = malloc(sizeof(wish_game_object));
		Wish_Memory_Zero(mem, sizeof(wish_game_object));
		wish_game_object* wgo = new (mem)wish_game_object(name);
		return wgo;
	}

	wish_transform* Wish_Scene_NewTransform(const char* name) {
		void* mem = malloc(sizeof(wish_transform));
		Wish_Memory_Zero(mem, sizeof(wish_transform));
		wish_transform* wt = new (mem)wish_transform(name);
		return wt;
	}

	wish_geometry* Wish_Scene_NewGeometry(const char* name) {
		void* mem = malloc(sizeof(wish_geometry));
		Wish_Memory_Zero(mem, sizeof(wish_geometry));
		wish_geometry* wt = new (mem)wish_geometry(name);
		return wt;
	}

	wish_point_light* Wish_Scene_NewPointLight(const char* name) {
		void* mem = malloc(sizeof(wish_point_light));
		Wish_Memory_Zero(mem, sizeof(wish_point_light));
		wish_point_light* light = new (mem)wish_point_light(name);
		return light;
	}

	wish_light* Wish_Scene_NewDirectionalLight(const char* name) {
		void* mem = malloc(sizeof(wish_light));
		Wish_Memory_Zero(mem, sizeof(wish_light));
		wish_light* light = new (mem) wish_light(name);
		return light;
	}

}
void Wish::Wish_Scene_Init()
{
	wish_scene_context& scene = Wish_Engine_GetContext()->Scene;
	scene.Root = Wish_Scene_NewTransform("Root");

	Wish_Memory_Zero(&scene.Camera, sizeof(wish_camera));
	//Wish_Transform_Init(&scene.camera.Transform);
	scene.Camera.Projection = mat4(1.0f);
	scene.Camera.View = mat4(1.0f);
	scene.Camera.ViewProjection = mat4(1.0f);
}

wish_transform* Wish::Wish_Scene_GetRoot()
{
	wish_scene_context& scene = Wish_Engine_GetContext()->Scene;
	return scene.Root;
}

wish_camera* Wish::Wish_Scene_GetCamera()
{
	wish_scene_context& scene = Wish_Engine_GetContext()->Scene;
	return &scene.Camera;
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
		gameObject->OnUpdate();
		gameObject->OnFixedUpdate();
		gameObject->OnDraw();
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
	Wish_Renderer_SetCamera(&scene.Camera);

	//Here is the genius part!
	//This doesn't do any rendering, but submits() whatever needs to be rendered, while at the same time
	//Doing the processing of each node
	//printf("root children: %s\n", std::to_string(scene.root->Children[0]->Children.size()).c_str());
	recurDepth = 0;
	ProcessNode(scene.Root);
}