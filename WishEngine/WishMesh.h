#pragma once

#define MAX_BUFFERS 5

namespace Wish
{

	struct Vertex_VT
	{
		vec3 position;
		vec2 uv;
	};

	struct Vertex_VNT
	{
		vec3 position;
		vec3 normal;
		vec2 uv;
	};

	struct Vertex_VNTBW
	{
		vec3 position;
		vec3 normal;
		vec2 uv;
		GLint bones[4];
		GLfloat weights[4];
	};

	enum wish_mesh_type
	{
		WISH_VERTEX_VT,
		WISH_VERTEX_VNT,
		WISH_VERTEX_VNTBW
	};

	struct wish_rect
	{
		union {
			struct {
				v3 min;
				v3 max;
			};
			struct {
				r32 x0, y0, z0;
				r32 x1, y1, z1;
			};
		};
	};

	class wish_mesh
	{
	private:
		void CalculateBounds();

	public:

		r32* Vertices;
		u32 VertexSize;
		u32 NumVertices;

		u32* Indices;
		u32 NumIndices;

		u32 Buffers[MAX_BUFFERS];
		u32 VAO;
		u32 IBO;

		wish_mesh_type MeshType;
		b32 IsCompiled;
		wish_rect Bounds;

		__Wish_Export wish_mesh();
		__Wish_Export ~wish_mesh();

		//DEPRECATED
		__Wish_Export void SetVertices(r32* vertices, u32 numVertices, u32 vertexSize);
		__Wish_Export void SetVertices(r32* vertices, u32 numVertices);

		//
		__Wish_Export void SetVertices(r32* vertices, u32 numVertices, wish_mesh_type meshType);

		__Wish_Export void SetIndices(u32* indices, u32 numIndices);
		void DeleteBuffers();
		__Wish_Export b32 Compile();
		void Draw();
	};

	__Wish_Export wish_mesh* Wish_Mesh_Create();
}