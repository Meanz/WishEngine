#pragma once

//To be removed
#include "WishTiming.h"

namespace Wish
{

#define MAX_TEXTURES 8

	enum RenderPass
	{
		GEOMETRY = 0,
		LIGHTING = 1,
	};

	struct wish_render_texture
	{
		wish_framebuffer Framebuffer;
		wish_texture Texture;
	};

	void Wish_RenderTexture_Create(wish_render_texture* renderTexture, GLuint width, GLuint height);
	void Wish_RenderTexture_Prepare(wish_render_texture* renderTexture);

	struct GeometryBuffer
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
		vec3 kernel[KERNEL_SIZE];
		bool isKernelDefined = false;

		wish_camera* m_activeCamera;
		bool m_bInit, m_DeferredRendering;
		wish_shader_program* m_pCurrentShaderProgram;
		wish_timer m_Timer;

		wish_timer m_Timer_GBuffer;
		wish_timer m_Timer_PostProcessing;
		wish_timer m_Timer_LastBlit;
		wish_timer m_Timer_Flush;


		//DEF RENDERING
		GeometryBuffer GeometryBuffer;
		wish_render_texture m_SSAOTexture;
		wish_render_texture m_SSAOBlurredTexture;

		wish_render_texture m_OffScreenA;
		wish_render_texture m_OffScreenB;

		wish_shader_program* m_pDeferredShader;
		wish_shader_program* m_pDeferredRenderingShader;
		wish_shader_program* m_pSimpleShader;
		wish_shader_program* m_pSimpleUnlitShader;
		wish_shader_program* m_pSSAOShader;
		wish_shader_program* m_pBlurShader;

		wish_shader_program* m_pPointLightShader;
		wish_shader_program* m_pDirectionalLightShader;
		wish_shader_program* m_pNullShader;

		wish_mesh* m_Rect;
		wish_mesh* m_Sphere;

#define WISH_MAX_GEOMETRY_QUEUE_SIZE 4096
#define WISH_MAX_LIGHT_QUEUE_SIZE 128

		u32 NumGeometries;
		wish_geometry* GeometryPassQueue[WISH_MAX_GEOMETRY_QUEUE_SIZE];

		u32 NumLights;
		wish_light* LighPassQueue[WISH_MAX_LIGHT_QUEUE_SIZE];

		//
		wish_texture* m_BoundTextures[MAX_TEXTURES];

		//Look into refercing and not copying!!
		mat4 m_WorldMatrix;

		//###############
		//# PUBLIC VARS #
		//###############
		bool ShowSSAO, ShowTextures;
		GLuint m_Width, m_Height;

	};

	
	void Wish_Renderer_Init(int width, int height);
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

	void Wish_Renderer_ApplyMaterial(wish_shader_program* pShaderProgram, wish_material* pMaterial);
}