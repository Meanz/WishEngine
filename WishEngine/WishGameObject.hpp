#pragma once

namespace Wish
{
	struct wish_game_object
	{
		wish_transform Transform;

		//
		Wish_Void_Func OnFixedUpdate;
		Wish_Void_Func OnUpdate;
		Wish_Void_Func OnDraw;

		//For class suport
		void* Obj;
		Wish_Class_Func cOnFixedUpdate;
		Wish_Class_Func cOnUpdate;
		Wish_Class_Func cOnDraw;
	};

#define WISH_GEOMETRY_MAX_MESHES 50

	struct wish_geometry
	{
		wish_transform Transform;
		wish_mesh* Meshes[WISH_GEOMETRY_MAX_MESHES];
		wish_material Material;
	};


	__Wish_Export void Wish_Geometry_AddMesh(wish_geometry* geometry, wish_mesh* mesh);
	void Wish_Geometry_Draw(wish_geometry* geometry);

}