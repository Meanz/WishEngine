#pragma once

#define MAX_BUFFERS 4

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

	struct wish_mesh
	{
		GLfloat* m_pVertices;
		GLuint m_VertexSize;
		GLuint m_NumVertices;

		GLuint* m_pIndices;
		GLuint m_NumIndices;

		GLuint m_Buffers[MAX_BUFFERS];
		GLuint m_VAO;
		GLuint m_IBO;

		GLboolean m_IsCompiled;
	};

	__Wish_Export void Wish_Mesh_Create();

	__Wish_Export void Wish_Mesh_SetVertices(wish_mesh* mesh, GLfloat* vertices, GLuint numVertices, GLuint vertexSize);
	__Wish_Export void Wish_Mesh_SetIndices(wish_mesh* mesh, GLuint* indices, GLuint numIndices);

	__Wish_Export void Wish_Mesh_DeleteBuffers(wish_mesh* mesh);
	__Wish_Export void Wish_Mesh_Compile(wish_mesh* mesh);
	__Wish_Export void Wish_Mesh_Draw(wish_mesh* mesh);

}