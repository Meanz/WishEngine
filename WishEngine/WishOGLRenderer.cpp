#include "stdafx.h"
#include "Wish.h"

//Include all OGL related things
#include "WishOGLTexture.cpp"
#include "WishOGLShader.cpp"
#include "WishOGLMaterial.cpp"
#include "WishOGLMesh.cpp"

//
#include "WishOGLFramebuffer.cpp"
#include "WishOGLDeferredRenderer.cpp"

//Meh
#include "WishOBJLoader.h"
#include "WishFontRenderer.hpp"

FreetypeFont m_DebugFont;

void Wish::Wish_Renderer_Init(int width, int height)
{
	wish_renderer_context& renderer = Wish_Engine_GetContext()->Renderer;
	renderer.ActiveCamera = NULL;
	renderer.DeferredRendering = true;
	renderer.ShowTextures = true;
	renderer.WorldMatrix = mat4(1.0f);

	renderer.Width = width;
	renderer.Height = height;

	Wish_DeferredRenderer_Init(&renderer.GeometryBuffer, width, height);

	Wish_RenderTexture_Create(&renderer.m_SSAOTexture, width, height);
	Wish_RenderTexture_Create(&renderer.m_SSAOBlurredTexture, width, height);

	//Wish_RenderTexture_Create(&renderer.m_OffScreenA, width, height);
	//Wish_RenderTexture_Create(&renderer.m_OffScreenB, width, height);



	//Let's populate our rect
	renderer.Rect = Wish_Primitive_Quad(-1, 1, -1, 1, 1, 1);

	renderer.DeferredShader = Wish_Asset_LoadShader("deferredShading", "./data/shaders/deferredShading.glsl");
	renderer.DeferredRenderingShader = Wish_Asset_LoadShader("deferredRendering", "./data/shaders/deferredRendering.glsl");

	renderer.SimpleShader = Wish_Asset_LoadShader("simple", "./data/shaders/simple.glsl");
	renderer.SimpleUnlitShader = Wish_Asset_LoadShader("simple_unlit", "./data/shaders/unlit_simple.glsl");

	renderer.SSAOShader = Wish_Asset_LoadShader("ssao", "./data/shaders/ssao.glsl");
	renderer.BlurShader = Wish_Asset_LoadShader("blur", "./data/shaders/blur.glsl");

	renderer.PointLightShader = Wish_Asset_LoadShader("point_light", "./data/shaders/pointlight.glsl");
	renderer.DirectionalLightShader = Wish_Asset_LoadShader("directional_light", "./data/shaders/directionallight.glsl");

	renderer.NullShader = Wish_Asset_LoadShader("null", "./data/shaders/nulltech.glsl");

	m_DebugFont.LoadFont("./data/fonts/arial.ttf", 14);
	renderer.Sphere = WishOBJLoader_Load("./data/models/sphere.obj");


	renderer.m_bInit = true;
}

namespace Wish
{


	void Wish_Renderer_SetWorldMatrix(const mat4& other)
	{
		wish_renderer_context& renderer = Wish_Engine_GetContext()->Renderer;
		renderer.WorldMatrix = other;
	}

	const mat4& Wish_Renderer_GetWorldMatrix() {
		wish_renderer_context& renderer = Wish_Engine_GetContext()->Renderer;
		return renderer.WorldMatrix;
	}

}

wish_shader_program* Wish::Wish_Renderer_GetShaderProgram() {
	wish_renderer_context& renderer = Wish_Engine_GetContext()->Renderer;
	return renderer.CurrentShaderProgram;
}

void Wish::Wish_Renderer_SetShaderProgram(wish_shader_program* pShaderProgram) {
	wish_renderer_context& renderer = Wish_Engine_GetContext()->Renderer;
	renderer.CurrentShaderProgram = pShaderProgram;
	Wish_ShaderProgram_Bind(pShaderProgram);
}


wish_camera* Wish::Wish_Renderer_GetCamera() {
	wish_renderer_context& renderer = Wish_Engine_GetContext()->Renderer;
	return renderer.ActiveCamera;
}

void Wish::Wish_Renderer_SetCamera(wish_camera* camera) {
	wish_renderer_context& renderer = Wish_Engine_GetContext()->Renderer;
	renderer.ActiveCamera = camera;
}

GLuint Wish::Wish_Renderer_GetWidth() {
	wish_renderer_context& renderer = Wish_Engine_GetContext()->Renderer;
	return renderer.Width;
}

GLuint Wish::Wish_Renderer_GetHeight() {
	wish_renderer_context& renderer = Wish_Engine_GetContext()->Renderer;
	return renderer.Height;
}

namespace Wish
{
	void Wish_Renderer_Submit(wish_geometry* geometry) {
		wish_renderer_context& renderer = Wish_Engine_GetContext()->Renderer;
		renderer.GeometryPassQueue[renderer.NumGeometries++] = geometry;
	}
	void Wish_Renderer_Submit(wish_light* light) {
		wish_renderer_context& renderer = Wish_Engine_GetContext()->Renderer;
		renderer.LighPassQueue[renderer.NumLights++] = light;
	}
}

void Wish::Wish_Renderer_BindTexture(GLuint slot, wish_texture* pTexture) {
	wish_renderer_context& renderer = Wish_Engine_GetContext()->Renderer;
	//Slot range check
	if (slot < 0 || slot >= MAX_TEXTURES) {
		assert(false);
		return;
	}

	//Do match match
	if (renderer.m_BoundTextures[slot] == pTexture) {
		//Do nothing!
		return;
	}
	//Otherwise swap swhit out
	renderer.m_BoundTextures[slot] = pTexture;

	if (pTexture == NULL) {
		Wish_Texture_Bind(NULL, GL_TEXTURE0 + slot);
		Wish_Texture_Disable();
	}
	else {
		Wish_Texture_Bind(pTexture, GL_TEXTURE0 + slot);
		Wish_Texture_Enable();
	}

}

void PreLightingPasses()
{
	wish_renderer_context& renderer = Wish_Engine_GetContext()->Renderer;
	//Wish_RenderTexture_Prepare(&renderer.m_SSAOTexture);

	Wish_Framebuffer_BindRead(&renderer.GeometryBuffer.Framebuffer);
	Wish_Framebuffer_BindWrite(&renderer.m_SSAOTexture.Framebuffer);
	glClear(GL_COLOR_BUFFER_BIT);

	Wish_Renderer_SetShaderProgram(renderer.SSAOShader);
	Wish_Renderer_ApplyUniforms(renderer.SSAOShader, nullptr, nullptr);
	renderer.Rect->Draw();
}

void PostLightingPasses()
{
	/*//Draw gbuffer contents
	Framebuffer::Bind(0);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//Draw the last quad!
	SetShaderProgram(DeferredRenderingShader);
	{
	Texture::Enable();
	GeometryBuffer.Diffuse.Bind(GL_TEXTURE0);
	GeometryBuffer.Positions.Bind(GL_TEXTURE1);
	GeometryBuffer.Normals.Bind(GL_TEXTURE2);
	m_SSAOBlurredTexture.GetTexture().Bind(GL_TEXTURE3);

	ApplyUniforms(DeferredRenderingShader, this, nullptr);
	m_Rect.Draw(this);

	//Disable
	Texture::Bind(GL_TEXTURE0, 0);
	Texture::Bind(GL_TEXTURE1, 0);
	Texture::Bind(GL_TEXTURE2, 0);
	Texture::Bind(GL_TEXTURE3, 0);
	Texture::Disable();
	}
	SetShaderProgram(NULL);*/
}

//Renderer does not need to know about scene, ooh yeah baby
void Wish::Wish_Renderer_Flush() {
	wish_renderer_context& renderer = Wish_Engine_GetContext()->Renderer;
	Wish_Timer_Start(&renderer.m_Timer_Flush);
	if (renderer.DeferredRendering) {

		//GBuffer this one is fairly simple!
		Wish_Timer_Start(&renderer.m_Timer_GBuffer);
		{
			Wish_DeferredRenderer_GeometryPass();
		}
		Wish_Timer_Stop(&renderer.m_Timer_GBuffer);

		Wish_Timer_Start(&renderer.m_Timer_PostProcessing);
		{
			//Post Processing

			//Create SSAO Image from GBuffer

			//Apply SSAO in lighting calculations using SSAO sampling?

			//Bind our Gbuffer for reading
			PreLightingPasses();

			Wish_DeferredRenderer_LightingPass();

			PostLightingPasses();

		}
		Wish_Timer_Stop(&renderer.m_Timer_PostProcessing);

		//Bind 0 for both read and write
		Wish_Framebuffer_BindWrite(NULL);
		Wish_Framebuffer_BindRead(NULL);

		//If we are debugging
		if (renderer.ShowTextures) {
			Wish_DeferredRenderer_DrawDebugTextures();
		}
	}
	else {
		Wish_Renderer_SetShaderProgram(renderer.SimpleShader);
		{
			// Turn on wireframe mode
			glDisable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_TRUE);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			Wish_Renderer_ApplyUniforms(renderer.SimpleShader, nullptr, nullptr);

			//Process render queue
			for (size_t i = 0; i < renderer.NumGeometries; i++) {
				wish_geometry* pGameObject = renderer.GeometryPassQueue[i];
				{
					pGameObject->Draw();
				}
			}

			glDisable(GL_DEPTH_TEST);
			glDepthMask(GL_FALSE);

			//Render our sphere at some location
			//Should put it under the cubes
			renderer.WorldMatrix = glm::translate(mat4(1.0f), vec3(-5, -5, -5)) * glm::scale(mat4(1.0f), vec3(0.1f, 0.1f, 0.1f));
			Wish_Renderer_ApplyUniforms(renderer.SimpleShader, nullptr, nullptr);

			renderer.Rect->Draw();
		}
		Wish_Renderer_SetShaderProgram(NULL);
	}
	Wish_Timer_Stop(&renderer.m_Timer_Flush);

	//Legacy draw
	Wish_Framebuffer_Bind(NULL);
	Wish_Renderer_SetShaderProgram(NULL);

	//Disable other texture slots?
	Wish_Texture_Bind(NULL, GL_TEXTURE0);
	Wish_Texture_Bind(NULL, GL_TEXTURE1);
	Wish_Texture_Bind(NULL, GL_TEXTURE2);
	Wish_Texture_Bind(NULL, GL_TEXTURE3);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0f, renderer.Width, renderer.Height, 0.0f, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	//Draw font thing here
	glColor3f(1.0f, 1.0f, 1.0f);
	Wish_Texture_Bind(NULL, GL_TEXTURE0);

	m_DebugFont.GLDraw("FPS: " + std::to_string(Wish_Engine_GetFPS()), 10, 10);
	m_DebugFont.GLDraw("Flush: " + std::to_string(Wish_Timer_GetValue(&renderer.m_Timer_Flush)), 10, 25);
	m_DebugFont.GLDraw("GBuffer: " + std::to_string(Wish_Timer_GetValue(&renderer.m_Timer_GBuffer)), 10, 40);
	m_DebugFont.GLDraw("PostProcessing: " + std::to_string(Wish_Timer_GetValue(&renderer.m_Timer_PostProcessing)), 10, 55);
	m_DebugFont.GLDraw("Geometry Objects: " + std::to_string(renderer.NumGeometries), 10, 70);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	//Clear all render queues, regardless
	renderer.NumGeometries = 0;
	renderer.NumLights = 0;
}

//TODO(Meanzie): Revise
void Wish::Wish_Renderer_DrawMesh(wish_mesh* mesh, wish_material* material) {
	//Ugh, but the renderer decides the shader :o

	//Apply material
	Wish_Renderer_ApplyMaterial(Wish_Renderer_GetShaderProgram(), material);
	mesh->Draw();
}