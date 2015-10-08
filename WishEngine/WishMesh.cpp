#include "stdafx.h"
#include "WishCore.h"
#include "WishMesh.hpp"

void Wish::Wish_Mesh_SetVertices(wish_mesh* mesh, GLfloat* vertices, GLuint numVertices, GLuint vertexSize) {
	mesh->m_pVertices = vertices;
	mesh->m_NumVertices = numVertices;
	mesh->m_VertexSize = vertexSize;
}

void Wish::Wish_Mesh_SetIndices(wish_mesh* mesh, GLuint* indices, GLuint numIndices) {
	mesh->m_pIndices = indices;
	mesh->m_NumIndices = numIndices;
	mesh->m_IsCompiled = false; //Setting indices will destroy our drawcall, so make sure to flag this as not compiled
}

void Wish::Wish_Mesh_DeleteBuffers(wish_mesh* mesh) {
	if (mesh->m_VAO != 0)
	{
		glDeleteVertexArrays(1, &mesh->m_VAO);
	}
	for (int i = 0; i < MAX_BUFFERS; i++) {
		if (mesh->m_Buffers[i] != 0)
		{
			glDeleteBuffers(1, &mesh->m_Buffers[i]);
		}
	}
}

void Wish::Wish_Mesh_Compile(wish_mesh* mesh) {
	Wish_Mesh_DeleteBuffers(mesh);

	//Generate the buffers
	//Needs information about how to compile? 
	//For now let's just make one buffer per thingy

	glGenVertexArrays(1, &mesh->m_VAO);
	glBindVertexArray(mesh->m_VAO);
	{
		glGenBuffers(1, mesh->m_Buffers);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->m_Buffers[0]);

		//Buffer the data I assume?
		glBufferData(
			GL_ARRAY_BUFFER,
			(GLsizeiptr)(mesh->m_NumVertices * mesh->m_VertexSize),
			(void*)mesh->m_pVertices,
			GL_STATIC_DRAW);

		//Assumption is VNT
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, mesh->m_VertexSize, (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, mesh->m_VertexSize, (void*)(3 * sizeof(GLfloat)));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, mesh->m_VertexSize, (void*)(6 * sizeof(GLfloat)));

		glGenBuffers(1, &mesh->m_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->m_IBO);

		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			(GLsizeiptr)(mesh->m_NumIndices * sizeof(GLuint)),
			(void*)mesh->m_pIndices,
			GL_STATIC_DRAW);
	}
	glBindVertexArray(0);

	mesh->m_IsCompiled = true; //Flag as compiled
}

void Wish::Wish_Mesh_Draw(wish_mesh* mesh) {
	if (!mesh->m_IsCompiled) {
		ASSERT(false);
	}

	glBindVertexArray(mesh->m_VAO);
	{
		glDrawElements(GL_TRIANGLES, mesh->m_NumIndices, GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);
}
