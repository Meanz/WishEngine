#pragma once

namespace Wish
{
	#define WISH_GEOMETRY_MAX_MESHES 50

	class WishGeometry : public WishTransform
	{

	public:
		wish_mesh* Meshes[WISH_GEOMETRY_MAX_MESHES];
		wish_material Material;
	
		__Wish_Export WishGeometry(const char* name);
		__Wish_Export ~WishGeometry();

		__Wish_Export void AddMesh(wish_mesh* mesh);
		void Draw();
	};
}
