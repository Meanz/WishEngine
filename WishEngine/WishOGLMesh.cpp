namespace Wish
{

	u32 GetVertexSize(wish_mesh_type type)
	{
		u32 result = 0;
		if (type == wish_mesh_type::WISH_VERTEX_VT) {
			result = sizeof(Vertex_VT);
		}
		else if (type == wish_mesh_type::WISH_VERTEX_VNT) {
			result = sizeof(Vertex_VNT);
		}
		else if (type == wish_mesh_type::WISH_VERTEX_VNTBW) {
			result = sizeof(Vertex_VNTBW);
		}
		return result;
	}

	wish_mesh::wish_mesh()
	{
		//Default to vnt
		MeshType = WISH_VERTEX_VNT;
	}

	wish_mesh::~wish_mesh()
	{
		DeleteBuffers();
	}

	void wish_mesh::CalculateBounds()
	{
		if (NumVertices > 0) {
			Bounds.min = v3(1000000);
			Bounds.max = v3(-1000000);

			u32 vSize = GetVertexSize(MeshType);
			u8* offset = (u8*)Vertices;
			v3* position = (v3*)offset;
			for (u32 i = 0; i < NumVertices; i++)
			{
				position = (v3*)offset;
				if (position->x < Bounds.min.x) {
					Bounds.min.x = position->x;
				}
				if (position->x > Bounds.max.x) {
					Bounds.max.x = position->x;
				}
				if (position->y < Bounds.min.y) {
					Bounds.min.y = position->y;
				}
				if (position->y > Bounds.max.y) {
					Bounds.max.y = position->y;
				}
				if (position->z < Bounds.min.z) {
					Bounds.min.z = position->z;
				}
				if (position->z > Bounds.max.z) {
					Bounds.max.z = position->z;
				}
				offset += vSize;
			}
		}
		else {
			Bounds.min = v3(0);
			Bounds.max = v3(0);
		}
	}

	void wish_mesh::SetVertices(r32* vertices, u32 numVertices, u32 vertexSize)
	{
		Vertices = vertices;
		NumVertices = numVertices;
		VertexSize = vertexSize;
		CalculateBounds();
	}



	void wish_mesh::SetVertices(r32* vertices, u32 numVertices) {
		Vertices = vertices;
		NumVertices = numVertices;
		VertexSize = GetVertexSize(MeshType);
		CalculateBounds();
	}

	__Wish_Export void SetVertices(r32* vertices, u32 numVertices, wish_mesh_type meshType);


	void wish_mesh::SetIndices(u32* indices, u32 numIndices)
	{
		Indices = indices;
		NumIndices = numIndices;
		IsCompiled = false; //Setting indices will destroy our drawcall, so make sure to flag this as not compiled
	}

	void wish_mesh::DeleteBuffers()
	{
		//Delete stuff if it exists
		if (VAO != 0)
		{
			glDeleteVertexArrays(1, &VAO);
		}
		for (int i = 0; i < MAX_BUFFERS; i++) {
			if (Buffers[i] != 0)
			{
				glDeleteBuffers(1, &Buffers[i]);
			}
		}
		if (IBO != 0)
		{
			glDeleteBuffers(1, &IBO);
		}
	}

	b32 wish_mesh::Compile()
	{
		DeleteBuffers();

		//Generate the buffers
		//Needs information about how to compile? 
		//For now let's just make one buffer per thingy

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		{
			glGenBuffers(1, Buffers);
			glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);

			//Buffer the data I assume?
			glBufferData(
				GL_ARRAY_BUFFER,
				(GLsizeiptr)(NumVertices* VertexSize),
				(void*)Vertices,
				GL_STATIC_DRAW);

			//Assumption is VNT
			if (MeshType == WISH_VERTEX_VT)
			{
				//Position
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VertexSize, (void*)0);

				//Texture
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, VertexSize, (void*)(3 * sizeof(GLfloat)));
			}
			if (MeshType == WISH_VERTEX_VNT)
			{
				//Position
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VertexSize, (void*)0);

				//Normal
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VertexSize, (void*)(3 * sizeof(GLfloat)));

				//Texture
				glEnableVertexAttribArray(2);
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, VertexSize, (void*)(6 * sizeof(GLfloat)));
			}
			else if (MeshType == WISH_VERTEX_VNTBW)
			{
				//Position
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VertexSize, (void*)0);

				//Normal
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VertexSize, (void*)(3 * sizeof(GLfloat)));

				//UV
				glEnableVertexAttribArray(2);
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, VertexSize, (void*)(6 * sizeof(GLfloat)));

				//Bones
				glEnableVertexAttribArray(3);
				glVertexAttribPointer(3, 4, GL_INT, GL_FALSE, VertexSize, (void*)(8 * sizeof(GLfloat)));

				//Weights
				glEnableVertexAttribArray(3);
				glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, VertexSize, (void*)(8 * sizeof(GLfloat) + 4 * sizeof(GLint)));
			}

			glGenBuffers(1, &IBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

			glBufferData(
				GL_ELEMENT_ARRAY_BUFFER,
				(GLsizeiptr)(NumIndices * sizeof(GLuint)),
				(void*)Indices,
				GL_STATIC_DRAW);
		}
		glBindVertexArray(0);

		IsCompiled = true; //Flag as compiled
		return IsCompiled;
	}

	void wish_mesh::Draw()
	{
		if (!IsCompiled) {
			ASSERT(false);
		}

		glBindVertexArray(VAO);
		{
			glDrawElements(GL_TRIANGLES, NumIndices, GL_UNSIGNED_INT, 0);
		}
		glBindVertexArray(0);
	}

	wish_mesh* Wish_Mesh_Create()
	{
		void* mem = Wish_Memory_Alloc(sizeof(wish_mesh));
		//Zero mem?
		wish_mesh* mesh = new (mem)wish_mesh();
		return mesh;
	}
}
