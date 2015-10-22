#pragma once

#include "VoxelChunk.hpp"

class VoxelTerrain : public WishGameObject
{

private:

	//The chunks that we are rendering
	
	

public:
	std::vector<VoxelChunk*> m_vChunks;

	VoxelTerrain();
	~VoxelTerrain();

	virtual void OnUpdate();
};