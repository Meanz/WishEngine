#pragma once

namespace Wish
{
	#define WISH_GEOMETRY_MAX_MESHES 50

	class wish_geometry : public wish_transform
	{

	public:
		wish_mesh* Meshes[WISH_GEOMETRY_MAX_MESHES];
		wish_material Material;
	
		__Wish_Export wish_geometry(const char* name);
		__Wish_Export ~wish_geometry();

		__Wish_Export void AddMesh(wish_mesh* mesh);
		void Draw();
	};
}
