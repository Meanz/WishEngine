
namespace Wish
{

	wish_geometry::wish_geometry(const char* name) : wish_transform(name)
	{
		TransformType = WISH_TRANSFORM_TYPE_GEOMETRY;
	}

	wish_geometry::~wish_geometry()
	{

	}

	void wish_geometry::AddMesh(wish_mesh* mesh)
	{
		Meshes[0] = mesh;
	}

	void wish_geometry::Draw()
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

		//Nooooo
		CalculateGlobal(mat4(1.0));
		Wish_Renderer_SetWorldMatrix(Global);
		//Tell our mesh renderer that it has the go ahead
		for (size_t i = 0; i < WISH_GEOMETRY_MAX_MESHES; i++) {
			if (Meshes[i] != NULL)
			{
					Wish_Renderer_DrawMesh(Meshes[i], &Material);
			}
		}
	}

}