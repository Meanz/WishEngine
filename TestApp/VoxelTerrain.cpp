#include "stdafx.h"
#include "VoxelTerrain.hpp"

void OnUpdate(void* obj)
{
	VoxelTerrain* vt = (VoxelTerrain*)obj;

	//Make some chunks
	if (vt->m_vChunks.size() == 0) {
		Wish_Asset_LoadTexture("wald", "./data/textures/wald.png");
		for (size_t x = 0; x < 8; x++) {
			for (size_t y = 0; y < 8; y++) {
				VoxelChunk* pChunk = new VoxelChunk(x, y);
				vt->m_vChunks.push_back(0);
				Wish_Transform_Attach(&vt->transf->Transform, &pChunk->geometry->Transform);
				printf("Attached transform.");
				pChunk->Build();
			}
		}
	}
}

VoxelTerrain::VoxelTerrain() {
	transf = Wish_Scene_NewGameObject("VoxelTerrain");
	transf->cOnUpdate = &OnUpdate;
	transf->Obj = this;
}