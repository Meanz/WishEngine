#include "stdafx.h"
#include "VoxelChunk.hpp"
#include <iostream>

#define MAX_HEIGHT 32

/** Use to init the clock */
#define TIMER_INIT \
    LARGE_INTEGER frequency; \
    LARGE_INTEGER t1,t2; \
    double elapsedTime; \
    QueryPerformanceFrequency(&frequency);


/** Use to start the performance timer */
#define TIMER_START QueryPerformanceCounter(&t1);

/** Use to stop the performance timer and output the result to the standard stream. Less verbose than \c TIMER_STOP_VERBOSE */
#define TIMER_STOP \
    QueryPerformanceCounter(&t2); \
    elapsedTime=(float)(t2.QuadPart-t1.QuadPart)/(frequency.QuadPart/1000); \
    std::wcout<<elapsedTime<<L" ms"<<endl;

VoxelChunk::VoxelChunk(size_t x, size_t y) :
m_X(x),
m_Y(y),
m_Perlin(0)
{
	geometry = Wish_Scene_NewGeometry("VoxelChunk");
	geometry->Transform.Position = vec3(m_X * CHUNK_SIZE * QUAD_SIZE, 0.0f, m_Y * CHUNK_SIZE * QUAD_SIZE);
	Wish_Transform_CalculateGlobal(&geometry->Transform, mat4(1.0f));

	m_Perlin = new PerlinNoise(1337, 1.2, 0.006, 8, 4);
	//m_Perlin->amplitude = 100;
	m_HeightMap = new GLfloat[CHUNK_SIZE * CHUNK_SIZE];
	for (size_t i = 0; i < CHUNK_SIZE; i++) {
		for (size_t j = 0; j < CHUNK_SIZE; j++) {
			m_HeightMap[i + (j * CHUNK_SIZE)] = (GLfloat)m_Perlin->getHeight(i + m_X * CHUNK_SIZE, j + m_Y * CHUNK_SIZE) + 4;
		}
	}
	 
	m_Voxels = new int[CHUNK_SIZE * MAX_HEIGHT * CHUNK_SIZE];
	//Fill voxel map
	for (size_t x = 0; x < CHUNK_SIZE; x++) {
		for (size_t y = 0; y < MAX_HEIGHT; y++) {
			for (size_t z = 0; z < CHUNK_SIZE; z++) {
				if (y > m_HeightMap[x + (z * CHUNK_SIZE)]) {
					//Dont assign
					m_Voxels[x + (z * CHUNK_SIZE) + (y * CHUNK_SIZE * CHUNK_SIZE)] = 0;
				}
				else {
					m_Voxels[x + (z * CHUNK_SIZE) + (y * CHUNK_SIZE * CHUNK_SIZE)] = 1;
				}
			}
		}
	}

}

VoxelChunk::~VoxelChunk()
{
	if (m_Perlin != 0)
		delete m_Perlin;

	delete m_HeightMap;
}

GLfloat VoxelChunk::GetHeight(GLint x, GLint y) {
	if (x < 0 || y < 0 || x >= CHUNK_SIZE || y >= CHUNK_SIZE) {
		//Hax
		return (GLfloat)m_Perlin->getHeight(x + m_X * CHUNK_SIZE, y + m_Y * CHUNK_SIZE);
	}
	else{
		return m_HeightMap[x + (y * CHUNK_SIZE)];
	}

	/*if (m_Perlin == 0) {
		return 0.0f;
		}
		else {
		return (GLfloat)m_Perlin->getHeight(x + m_X * CHUNK_SIZE, y + m_Y * CHUNK_SIZE);
		}*/
}

inline vec3 calcNormal(vec3 p1, vec3 p2, vec3 p3)
{
	//U,V
	return glm::cross(p2 - p1, p3 - p1);
}

vec3 VoxelChunk::CalculateNormal(GLint x, GLint y) {
	//That's a lot of defines!
	vec3 p1 = vec3(x, GetHeight(x, y), y);
	vec3 p2 = vec3(x, GetHeight(x, y + 1), y + 1);
	vec3 p3 = vec3(x + 1, GetHeight(x + 1, y + 1), y + 1);
	vec3 p4 = vec3(x + 1, GetHeight(x + 1, y), y);
	vec3 p5 = vec3(x, GetHeight(x, y - 1), y - 1);
	vec3 p6 = vec3(x - 1, GetHeight(x - 1, y - 1), y - 1);
	vec3 p7 = vec3(x - 1, GetHeight(x - 1, y), y);
	vec3 p8 = vec3(x - 1, GetHeight(x - 1, y + 1), y + 1);
	vec3 p9 = vec3(x + 1, GetHeight(x + 1, y - 1), y - 1);

	vec3 norm1 = calcNormal(p1, p4, p3);
	vec3 norm2 = calcNormal(p3, p2, p1);

	vec3 norm3 = calcNormal(p7, p1, p2);
	vec3 norm4 = calcNormal(p2, p8, p7);

	vec3 norm5 = calcNormal(p6, p5, p1);
	vec3 norm6 = calcNormal(p1, p7, p6);

	vec3 norm7 = calcNormal(p4, p1, p5);
	vec3 norm8 = calcNormal(p5, p9, p4);

	float endX = norm1.x + norm2.x + norm3.x + norm4.x + norm5.x + norm6.x + norm7.x + norm8.x;
	float endY = norm1.y + norm2.y + norm3.y + norm4.y + norm5.y + norm6.y + norm7.y + norm8.y;
	float endZ = norm1.z + norm2.z + norm3.z + norm4.z + norm5.z + norm6.z + norm7.z + norm8.z;
	return vec3(endX / 8.0f, endY / 8.0f, endZ / 8.0f);
}

int VoxelChunk::GetVoxel(GLint x, GLint y, GLint z) {
	if (x < 0 || y < 0 || z < 0 || x >= CHUNK_SIZE || y >= MAX_HEIGHT || z >= CHUNK_SIZE) {
		return 0; //NUFIN
	}
	else {
		return m_Voxels[x + (z * CHUNK_SIZE) + (y * CHUNK_SIZE * CHUNK_SIZE)];
	}
}

void VoxelChunk::Build() {
	TIMER_INIT

#define DRAW_SMOOTH_TERRAIN 1

		TIMER_START
	size_t voff = 0;
	size_t ioff = 0;
#if DRAW_SMOOTH_TERRAIN == 1
	//Seed, persistence, frequency, amplitude, octaves
#define NUM_QUADS CHUNK_SIZE * CHUNK_SIZE
#define VERTICES_PER_QUAD 4
#define INDICES_PER_QUAD 6


	std::vector<Vertex_VNT> vertices = std::vector<Vertex_VNT>(NUM_QUADS * VERTICES_PER_QUAD);
	std::vector<GLuint> indices = std::vector<GLuint>(NUM_QUADS * INDICES_PER_QUAD);

	for (size_t x = 0; x < CHUNK_SIZE; x++) {
		for (size_t y = 0; y < CHUNK_SIZE; y++) {

			//Create the quad vertices
			float height0 = QUAD_SIZE * (float)GetHeight(x, y + 1);
			float height1 = QUAD_SIZE * (float)GetHeight(x + 1, y + 1);
			float height2 = QUAD_SIZE * (float)GetHeight(x + 1, y);
			float height3 = QUAD_SIZE * (float)GetHeight(x, y);

			vec3 p1 = vec3(x * QUAD_SIZE, height0, y * QUAD_SIZE + QUAD_SIZE);
			vec3 p2 = vec3(x * QUAD_SIZE + QUAD_SIZE, height1, y * QUAD_SIZE + QUAD_SIZE);
			vec3 p3 = vec3(x * QUAD_SIZE + QUAD_SIZE, height2, y * QUAD_SIZE);
			vec3 p4 = vec3(x * QUAD_SIZE, height3, y * QUAD_SIZE);

#define DO_CALC_NORMALS 1

#if DO_CALC_NORMALS == 0
			vec3 n1 = normalize(CalculateNormal(x, y + 1));
			vec3 n2 = normalize(CalculateNormal(x + 1, y + 1));
			vec3 n3 = normalize(CalculateNormal(x + 1, y));
			vec3 n4 = normalize(CalculateNormal(x, y));
#else
			vec3 n1 = vec3(0.0f);
			vec3 n2 = vec3(0.0f);
			vec3 n3 = vec3(0.0f);
			vec3 n4 = vec3(0.0f);
#endif
			vec2 u1 = vec2(0.0f, 1.0f);
			vec2 u2 = vec2(1.0f, 1.0f);
			vec2 u3 = vec2(1.0f, 0.0f);
			vec2 u4 = vec2(0.0f, 0.0f);

			size_t _voff = voff;

			vertices[voff++] = (Vertex_VNT{ p1, n1, u1 });
			vertices[voff++] = (Vertex_VNT{ p2, n2, u2 });
			vertices[voff++] = (Vertex_VNT{ p3, n3, u3 });
			vertices[voff++] = (Vertex_VNT{ p4, n4, u4 });

			indices[ioff++] = _voff + 0;
			indices[ioff++] = _voff + 1;
			indices[ioff++] = _voff + 2;

			indices[ioff++] = _voff + 2;
			indices[ioff++] = _voff + 3;
			indices[ioff++] = _voff + 0;

		}
	}

	//Do normal smoothing
	size_t vertexCount = indices.size();

	for( int i = 0; i < vertexCount; i += 3 )
	{
		// get the three vertices that make the faces
		glm::vec3 p0 = vertices[indices[i + 0]].position;
		glm::vec3 p1 = vertices[indices[i + 1]].position;
		glm::vec3 p2 = vertices[indices[i + 2]].position;

		glm::vec3 e1 = p1 - p0;
		glm::vec3 e2 = p2 - p0;
		glm::vec3 normal = glm::cross( e1, e2 );
		normal = glm::normalize(normal);

		// Store the face's normal for each of the vertices that make up the face.
		vertices[indices[i + 0]].normal += normal;
		vertices[indices[i + 1]].normal += normal;
		vertices[indices[i + 2]].normal += normal;
	}


	// Now loop through each vertex vector, and avarage out all the normals stored.
	for( int i = 0; i < vertices.size(); ++i )
	{
		vertices[i].normal = glm::normalize(vertices[i].normal);
	}

#else
	//This will be expensive, oh boy, let's count number of blocks to be rendered first
	size_t numBlocksToRender = 0;
	size_t numFacesToRender = 0;


	for (size_t x = 0; x < CHUNK_SIZE; x++) {
		for (size_t y = 0; y < MAX_HEIGHT; y++) {
			for (size_t z = 0; z < CHUNK_SIZE; z++) {
				if (GetVoxel(x, y, z) == 1) {

					//Front face
					if (GetVoxel(x, y, z - 1) == 0) {
						numFacesToRender++;
					}

					//Right face
					if (GetVoxel(x - 1, y, z) == 0) {
						numFacesToRender++;
					}

					//Left face
					if (GetVoxel(x + 1, y, z) == 0) {
						numFacesToRender++;
					}

					//Back face
					if (GetVoxel(x, y, z + 1) == 0) {
						numFacesToRender++;
					}

					//Top face
					if (GetVoxel(x, y + 1, z) == 0) {
						numFacesToRender++;
					}

					//Bottom face
					if (GetVoxel(x, y - 1, z) == 0) {
						numFacesToRender++;
					}

				}
			}
		}
	}

	std::vector<Vertex_VNT> vertices = std::vector<Vertex_VNT>(numFacesToRender * 4);
	std::vector<GLuint> indices = std::vector<GLuint>(numFacesToRender * 6);

	for (size_t x = 0; x < CHUNK_SIZE; x++) {
		for (size_t y = 0; y < MAX_HEIGHT; y++) {
			for (size_t z = 0; z < CHUNK_SIZE; z++) {
				if (GetVoxel(x, y, z) == 1) {

					size_t px = (size_t) (x * QUAD_SIZE);
					size_t py = (size_t) (y * QUAD_SIZE);
					size_t pz = (size_t) (z * QUAD_SIZE);

					//Draw the block
					vec3 p1 = vec3(px, py, pz);
					vec3 p2 = vec3(px, py, pz + QUAD_SIZE);
					vec3 p3 = vec3(px + QUAD_SIZE, py, pz + QUAD_SIZE);
					vec3 p4 = vec3(px + QUAD_SIZE, py, pz);

					vec3 p5 = vec3(px + QUAD_SIZE, py + QUAD_SIZE, pz);
					vec3 p6 = vec3(px, py + QUAD_SIZE, pz);
					vec3 p7 = vec3(px, py + QUAD_SIZE, pz + QUAD_SIZE);
					vec3 p8 = vec3(px + QUAD_SIZE, py + QUAD_SIZE, pz + QUAD_SIZE);

					vec2 u1 = vec2(0.0f, 1.0f);
					vec2 u2 = vec2(1.0f, 1.0f);
					vec2 u3 = vec2(1.0f, 0.0f);
					vec2 u4 = vec2(0.0f, 0.0f);

					vec3 nFront = vec3(0.0f, 0.0f, -1.0f); //Front
					vec3 nRight = vec3(1.0f, 0.0f, 0.0f); //Right
					vec3 nLeft = vec3(-1.0f, 0.0f, 0.0f); //Left
					vec3 nBack = vec3(0.0f, 0.0f, 1.0f); //Back
					vec3 nTop = vec3(0.0f, 1.0f, 0.0f); //Top
					vec3 nBottom = vec3(0.0f, -1.0f, 0.0f); //Bottom

					size_t _voff = voff - 1;

					//Render front face
					if (GetVoxel(x, y, z - 1) == 0) {

						vertices[voff++] = (Vertex_VNT{ p4, nFront, u1 });
						vertices[voff++] = (Vertex_VNT{ p1, nFront, u2 });
						vertices[voff++] = (Vertex_VNT{ p6, nFront, u3 });
						vertices[voff++] = (Vertex_VNT{ p5, nFront, u4 });

						indices[ioff++] = _voff + 1;
						indices[ioff++] = _voff + 2;
						indices[ioff++] = _voff + 3;

						indices[ioff++] = _voff + 3;
						indices[ioff++] = _voff + 4;
						indices[ioff++] = _voff + 1;
					}

					

					//Render right face
					if (GetVoxel(x - 1, y, z) == 0) {
						_voff = voff - 1;
						vertices[voff++] = (Vertex_VNT{ p1, nRight, u1 });
						vertices[voff++] = (Vertex_VNT{ p2, nRight, u2 });
						vertices[voff++] = (Vertex_VNT{ p7, nRight, u3 });
						vertices[voff++] = (Vertex_VNT{ p6, nRight, u4 });

						indices[ioff++] = _voff + 1;
						indices[ioff++] = _voff + 2;
						indices[ioff++] = _voff + 3;

						indices[ioff++] = _voff + 3;
						indices[ioff++] = _voff + 4;
						indices[ioff++] = _voff + 1;
					}

					//Render left face
					if (GetVoxel(x + 1, y, z) == 0) {
						_voff = voff - 1;
						vertices[voff++] = (Vertex_VNT{ p3, nLeft, u1 });
						vertices[voff++] = (Vertex_VNT{ p4, nLeft, u2 });
						vertices[voff++] = (Vertex_VNT{ p5, nLeft, u3 });
						vertices[voff++] = (Vertex_VNT{ p8, nLeft, u4 });

						indices[ioff++] = _voff + 1;
						indices[ioff++] = _voff + 2;
						indices[ioff++] = _voff + 3;

						indices[ioff++] = _voff + 3;
						indices[ioff++] = _voff + 4;
						indices[ioff++] = _voff + 1;
					}

					//Render back face
					if (GetVoxel(x, y, z + 1) == 0) {
						_voff = voff - 1;
						vertices[voff++] = (Vertex_VNT{ p2, nBack, u1 });
						vertices[voff++] = (Vertex_VNT{ p3, nBack, u2 });
						vertices[voff++] = (Vertex_VNT{ p8, nBack, u3 });
						vertices[voff++] = (Vertex_VNT{ p7, nBack, u4 });

						indices[ioff++] = _voff + 1;
						indices[ioff++] = _voff + 2;
						indices[ioff++] = _voff + 3;

						indices[ioff++] = _voff + 3;
						indices[ioff++] = _voff + 4;
						indices[ioff++] = _voff + 1;
					}

					//Render top face
					if (GetVoxel(x, y + 1, z) == 0) {
						_voff = voff - 1;
						vertices[voff++] = (Vertex_VNT{ p5, nTop, u1 });
						vertices[voff++] = (Vertex_VNT{ p6, nTop, u2 });
						vertices[voff++] = (Vertex_VNT{ p7, nTop, u3 });
						vertices[voff++] = (Vertex_VNT{ p8, nTop, u4 });

						indices[ioff++] = _voff + 1;
						indices[ioff++] = _voff + 2;
						indices[ioff++] = _voff + 3;

						indices[ioff++] = _voff + 3;
						indices[ioff++] = _voff + 4;
						indices[ioff++] = _voff + 1;
					}

					//Render bottom face
					if (GetVoxel(x, y - 1, z) == 0) {
						_voff = voff - 1;
						vertices[voff++] = (Vertex_VNT{ p1, nBottom, u1 });
						vertices[voff++] = (Vertex_VNT{ p4, nBottom, u2 });
						vertices[voff++] = (Vertex_VNT{ p3, nBottom, u3 });
						vertices[voff++] = (Vertex_VNT{ p2, nBottom, u4 });

						indices[ioff++] = _voff + 1;
						indices[ioff++] = _voff + 2;
						indices[ioff++] = _voff + 3;

						indices[ioff++] = _voff + 3;
						indices[ioff++] = _voff + 4;
						indices[ioff++] = _voff + 1;
					}

				}
			}
		}
	}

#endif

	Wish_Mesh_SetVertices(&pMesh, (GLfloat*)&vertices[0], vertices.size(), sizeof(Vertex_VNT));
	Wish_Mesh_SetIndices(&pMesh, (GLuint*)&indices[0], indices.size());
	Wish_Mesh_Compile(&pMesh);

	//Attach the compiled mesh to our geometry node
	geometry->Meshes[0] = (&pMesh);
	geometry->Material.Albedo = Wish_Asset_GetTexture("wald");
	//Wish_Material_Set(&geometry->Material, "Albedo", Wish_Asset_GetTexture("wald"));

	printf("Loaded Chunk %d/%d\n", m_X, m_Y);

	TIMER_STOP
}