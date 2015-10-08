#pragma once

#include "VoxelChunk.hpp"

class VoxelTerrain
{

private:

	//The chunks that we are rendering
	
	

public:
	std::vector<VoxelChunk*> m_vChunks;
	wish_game_object* transf;

	VoxelTerrain();
	~VoxelTerrain();
};