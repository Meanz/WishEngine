#include "stdafx.h"
#include "VoxelTerrain.hpp"

VoxelTerrain::VoxelTerrain() : wish_game_object("VoxelTerrain") {
}

VoxelTerrain::~VoxelTerrain()
{

}

void VoxelTerrain::OnUpdate()
{
	//Make some chunks
	if (m_vChunks.size() == 0) {
		Wish_Asset_LoadTexture("wald", "./data/textures/wald.png");
		for (size_t x = 0; x < 8; x++) {
			for (size_t y = 0; y < 8; y++) {
				VoxelChunk* pChunk = new VoxelChunk(x, y);
				m_vChunks.push_back(0);
				Attach(pChunk);
				printf("Attached transform.");
				pChunk->Build();
			}
		}
	}
}