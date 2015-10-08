#pragma once

namespace Wish
{
	struct wish_model_header
	{
		u32 Version;
		u32 NumMeshes;
		u32 NumMaterials;
	};

	struct wish_model_mesh
	{
		u32 NumVertices;
		r32* Vertices;
		r32* Normals;
		r32* UVs;

		u32 NumIndices;
		u32* Indices;
	};

	struct wish_model
	{
		wish_model_header header;
		wish_mesh* Meshes;
		wish_material* Materials;
	};

	b32 Wish_Model_Save(wish_model* model, const char* fileName);
	wish_model* Wish_Model_Load(const char* fileName);

}