#pragma once

namespace Wish
{


	void Wish_DeferredRenderer_Init(GeometryBuffer* geometryBuffer, u32 width, u32 height);
	void Wish_DeferredRenderer_Stuff();
	void Wish_DeferredRenderer_GeometryPass();
	void Wish_DeferredRenderer_LightingPass();
	void Wish_DeferredRenderer_PreLightingPass();
	void Wish_DeferredRenderer_PostLightingPass();
	void Wish_DeferredRenderer_DrawDebugTextures();


}