#include "stdafx.h"
#include "Wish.h"
#include "WishPrimitives.h"

namespace Wish
{
	void Wish_Primitive_Rect_VT(wish_mesh* mesh, r32 x, r32 y, r32 w, r32 h)
	{
		Vertex_VT vertices[4];
		u32 indices[6];
		//invert y
		i32 vh = 0;//Wish_Window_GetHeight();
		vertices[0] = Vertex_VT{ v3(x, vh - (y + h), 0.0), v2(0.0, 1.0) };
		vertices[1] = Vertex_VT{ v3(x + w, vh - (y + h), 0.0), v2(1.0, 1.0) };
		vertices[2] = Vertex_VT{ v3(x + w, vh - (y), 0.0), v2(1.0, 0.0) };
		vertices[3] = Vertex_VT{ v3(x, vh - (y), 0.0), v2(0.0, 0.0) };
		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
		indices[3] = 2;
		indices[4] = 3;
		indices[5] = 0;
		mesh->MeshType = WISH_VERTEX_VT;
		mesh->SetVertices((r32*)vertices, 4);
		mesh->SetIndices(indices, 6);
		//Or, deferr compilation
		mesh->Compile();
		//Delete data
		mesh->Vertices = 0;
		mesh->Indices = 0;
	}

	wish_mesh* Wish_Primitive_Quad(i32 xMin, i32 xMax, i32 yMin, i32 yMax, i32 u, i32 v)
	{
		//Allocate on heap
		Vertex_VNT* vertices = (Vertex_VNT*)Wish_Memory_Alloc(4 * sizeof(Vertex_VNT));

		//Should perhap support u16?
		u32* indices = (u32*)Wish_Memory_Alloc(6 * sizeof(u32));

		//
		//
		//	  Z
		//	  |
		//	  |
		//X----

		vertices[0] = {
			v3(xMin, yMin, 0),
			v3(0.0, 1.0, 0.0),
			v2(0.0, 0.0)
		};

		vertices[1] = {
			v3(xMax, yMin, 0),
			v3(0.0, 1.0, 0.0),
			v2(u, 0.0)
		};

		vertices[2] = {
			v3(xMax, yMax, 0),
			v3(0.0, 1.0, 0.0),
			v2(u, v)
		};

		vertices[3] = {
			v3(xMin, yMax, 0),
			v3(0.0, 1.0, 0.0),
			v2(0, v)
		};

		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
		indices[3] = 2;
		indices[4] = 3;
		indices[5] = 0;

		wish_mesh* mesh = Wish_Mesh_Create();
		mesh->MeshType = WISH_VERTEX_VNT;
		mesh->SetVertices((r32*)vertices, 4, sizeof(Vertex_VNT));
		mesh->SetIndices((u32*)indices, 6);
		mesh->Compile();
		//Delete data
		mesh->Vertices = 0;
		mesh->Indices = 0;
		Wish_Memory_Free(vertices);
		Wish_Memory_Free(indices);
		return mesh;
	}

	wish_mesh* Wish_Primitive_Plane(i32 width, i32 length)
	{
		return Wish_Primitive_Plane(width, length, 1, 1);
	}

	wish_mesh* Wish_Primitive_Plane(i32 width, i32 length, i32 u, i32 v)
	{
		//Allocate on heap
		Vertex_VNT* vertices = (Vertex_VNT*)Wish_Memory_Alloc(4 * sizeof(Vertex_VNT));

		//Should perhap support u16?
		u32* indices = (u32*)Wish_Memory_Alloc(6 * sizeof(u32));

		//
		//
		//	  Z
		//	  |
		//	  |
		//X----

		vertices[0] = {
			v3(width / 2, 0, -length / 2),
			v3(0.0, 1.0, 0.0),
			v2(0.0, 0.0)
		};

		vertices[1] = {
			v3(-width / 2, 0, -length / 2),
			v3(0.0, 1.0, 0.0),
			v2(u, 0.0)
		};

		vertices[2] = {
			v3(-width / 2, 0, length / 2),
			v3(0.0, 1.0, 0.0),
			v2(u, v)
		};

		vertices[3] = {
			v3(width / 2, 0, length / 2),
			v3(0.0, 1.0, 0.0),
			v2(0, v)
		};

		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
		indices[3] = 2;
		indices[4] = 3;
		indices[5] = 0;

		wish_mesh* mesh = Wish_Mesh_Create();
		mesh->MeshType = WISH_VERTEX_VNT;
		mesh->SetVertices((r32*)vertices, 4, sizeof(Vertex_VNT));
		mesh->SetIndices((u32*)indices, 6);
		mesh->Compile();
		//Delete data
		mesh->Vertices = 0;
		mesh->Indices = 0;
		Wish_Memory_Free(vertices);
		Wish_Memory_Free(indices);
		return mesh;
	}

	wish_mesh* Wish_Primitive_Cube(i32 width, i32 height, i32 length)
	{
		return NULL;
	}

	wish_mesh* Wish_Primitive_Cylinder(i32 radius, i32 height, u32 resolution)
	{
		return NULL;
	}

	wish_mesh* Wish_Primitive_Sphere(i32 radius, u32 resolution)
	{
		return NULL;
	}
}