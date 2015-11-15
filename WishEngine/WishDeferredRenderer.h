#pragma once

namespace Wish
{

	//Fwd decl
	struct geometry_buffer;

	void Wish_DeferredRenderer_Init(geometry_buffer* geometryBuffer, u32 width, u32 height);
	void Wish_DeferredRenderer_Stuff();
	void Wish_DeferredRenderer_GeometryPass();
	void Wish_DeferredRenderer_LightingPass();
	void Wish_DeferredRenderer_PreLightingPass();
	void Wish_DeferredRenderer_PostLightingPass();
	void Wish_DeferredRenderer_DrawDebugTextures();


}