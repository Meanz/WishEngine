#pragma once

#include "VoxelChunk.hpp"

class VoxelTerrain : public wish_game_object
{

private:

	//The chunks that we are rendering
	
	

public:
	std::vector<VoxelChunk*> m_vChunks;

	VoxelTerrain();
	~VoxelTerrain();

	virtual void OnUpdate();
};