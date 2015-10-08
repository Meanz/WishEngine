#include "stdafx.h"
#include "Wish.hpp"
#include "WishGameObject.hpp"

namespace Wish
{
	void Wish_Geometry_AddMesh(wish_geometry* geometry, wish_mesh* mesh)
	{
		geometry->Meshes[0] = mesh;
	}
}

void Wish::Wish_Geometry_Draw(wish_geometry* geometry)
{
	//Okay, we are getting a drawcall from the renderer
	//Make us of it

	//Tell the current shader our transform position

	//Temp fix
	/*if (&geometry->Transform.IsChanged)
	{
		Wish_Transform_CalculateGlobal(&geometry->Transform, mat4(1.0f));
	}*/
	//Wish_Material_Set(&geometry->Material, "World", geometry->Transform.Global);
	geometry->Material.World = &geometry->Transform.Global;

	//Tell our mesh renderer that it has the go ahead
	for (size_t i = 0; i < WISH_GEOMETRY_MAX_MESHES; i++) {
		if (geometry->Meshes[i] != NULL)
		{
			Wish_Renderer_DrawMesh(geometry->Meshes[i], &geometry->Material);
		}
	}
}