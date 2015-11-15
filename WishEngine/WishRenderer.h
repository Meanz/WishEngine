#pragma once

//
#include "WishTexture.h"
#include "WishShader.h"
#include "WishMaterial.h"
#include "WishMesh.h"

//
#include "WishFramebuffer.h"
#include "WishDeferredRenderer.h"

//
#include "WishTiming.h"

//
#include "WishPrimitives.h"

namespace Wish
{

	//Fwd decl
	class wish_camera;
	class wish_geometry;

#define MAX_TEXTURES 8

	enum RenderPass
	{
		GEOMETRY = 0,
		LIGHTING = 1,
	};

	struct geometry_buffer
	{
		wish_framebuffer Framebuffer;
		wish_texture Diffuse;
		wish_texture Positions;
		wish_texture Normals;
		wish_texture Depth;
	};

#define KERNEL_SIZE 128

	struct wish_renderer_context
	{

		//2.0!
		mat4 WorldMatrix;

		//

		vec3 kernel[KERNEL_SIZE];
		bool isKernelDefined = false;

		wish_camera* ActiveCamera;
		bool m_bInit, DeferredRendering;
		wish_shader_program* CurrentShaderProgram;
		wish_timer m_Timer;

		wish_timer m_Timer_GBuffer;
		wish_timer m_Timer_PostProcessing;
		wish_timer m_Timer_LastBlit;
		wish_timer m_Timer_Flush;


		//DEF RENDERING
		geometry_buffer GeometryBuffer;
		wish_render_texture m_SSAOTexture;
		wish_render_texture m_SSAOBlurredTexture;

		wish_render_texture m_OffScreenA;
		wish_render_texture m_OffScreenB;

		wish_shader_program* DeferredShader;
		wish_shader_program* DeferredRenderingShader;
		wish_shader_program* SimpleShader;
		wish_shader_program* SimpleUnlitShader;
		wish_shader_program* SSAOShader;
		wish_shader_program* BlurShader;

		wish_shader_program* PointLightShader;
		wish_shader_program* DirectionalLightShader;
		wish_shader_program* NullShader;

		wish_mesh* Rect;
		wish_mesh* Sphere;

#define WISH_MAX_GEOMETRY_QUEUE_SIZE 4096
#define WISH_MAX_LIGHT_QUEUE_SIZE 128

		u32 NumGeometries;
		wish_geometry* GeometryPassQueue[WISH_MAX_GEOMETRY_QUEUE_SIZE];

		u32 NumLights;
		wish_light* LighPassQueue[WISH_MAX_LIGHT_QUEUE_SIZE];

		//
		wish_texture* m_BoundTextures[MAX_TEXTURES];

		//###############
		//# PUBLIC VARS #
		//###############
		bool ShowSSAO, ShowTextures;
		GLuint Width, Height;

	};

	
	void Wish_Renderer_Init(int width, int height);

	void Wish_Renderer_SetWorldMatrix(const mat4& world);
	const mat4& Wish_Renderer_GetWorldMatrix();

	wish_shader_program* Wish_Renderer_GetShaderProgram();
	void Wish_Renderer_SetShaderProgram(wish_shader_program* pShaderProgram);

	wish_camera* Wish_Renderer_GetCamera();
	void Wish_Renderer_SetCamera(wish_camera* camera);

	GLuint Wish_Renderer_GetWidth();
	GLuint Wish_Renderer_GetHeight();

	void Wish_Renderer_Submit(wish_geometry* geometry);
	void Wish_Renderer_Submit(wish_light* light);

	void Wish_Renderer_BindTexture(GLuint textureSlot, wish_texture* pTexture);

	void Wish_Renderer_Flush();

	void Wish_Renderer_DrawMesh(wish_mesh* mesh, wish_material* material);

	void Wish_Renderer_ApplyUniforms(wish_shader_program* pShaderProgram, wish_material* pMaterial, wish_light* pLight);
	void Wish_Renderer_ApplyMaterial(wish_shader_program* pShaderProgram, wish_material* pMaterial);
}