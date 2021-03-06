#pragma once

#include <Perlin.hpp>

#define CHUNK_SIZE 16
#define QUAD_SIZE 1.0f

namespace Wish { class ShaderProgram; };

class VoxelChunk : public wish_geometry
{

private:
	size_t m_X, m_Y;
	PerlinNoise* m_Perlin;
	GLfloat* m_HeightMap;
	int* m_Voxels;
	wish_mesh pMesh;

public:
	VoxelChunk(size_t x, size_t y);
	~VoxelChunk();

	int GetVoxel(GLint x, GLint y, GLint z);

	inline vec3 CalculateNormal(GLint x, GLint y);
	GLfloat GetHeight(GLint x, GLint y);

	void Build();
	
};