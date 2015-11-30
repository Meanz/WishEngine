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

//:P
#include "WishFontRenderer.hpp"

//Meh
#include "WishOBJLoader.h"

namespace Wish
{

	wish_ui_window* dbgWnd;

	void wish_renderer::Init(int width, int height)
	{
		ActiveCamera = NULL;
		DeferredRendering = true;
		ShowTextures = true;
		WorldMatrix = mat4(1.0f);

		Width = width;
		Height = height;

		Wish_DeferredRenderer_Init(&GeometryBuffer, width, height);

		Wish_RenderTexture_Create(&m_SSAOTexture, width, height);
		Wish_RenderTexture_Create(&m_SSAOBlurredTexture, width, height);

		//Wish_RenderTexture_Create(&renderer.m_OffScreenA, width, height);
		//Wish_RenderTexture_Create(&renderer.m_OffScreenB, width, height);



		//Let's populate our rect
		Rect = Wish_Primitive_Quad(-1, 1, -1, 1, 1, 1);

		DeferredShader = Wish_Asset_LoadShader("deferredShading", "./data/shaders/deferredShading.glsl");
		DeferredRenderingShader = Wish_Asset_LoadShader("deferredRendering", "./data/shaders/deferredRendering.glsl");

		SimpleShader = Wish_Asset_LoadShader("simple", "./data/shaders/simple.glsl");
		SimpleUnlitShader = Wish_Asset_LoadShader("simple_unlit", "./data/shaders/unlit_simple.glsl");

		SSAOShader = Wish_Asset_LoadShader("ssao", "./data/shaders/ssao.glsl");
		BlurShader = Wish_Asset_LoadShader("blur", "./data/shaders/blur.glsl");

		PointLightShader = Wish_Asset_LoadShader("point_light", "./data/shaders/pointlight.glsl");
		DirectionalLightShader = Wish_Asset_LoadShader("directional_light", "./data/shaders/directionallight.glsl");

		NullShader = Wish_Asset_LoadShader("null", "./data/shaders/nulltech.glsl");

		Sphere = WishOBJLoader_Load("./data/models/sphere.obj");

		dbgWnd = new wish_ui_window(200, 300);

		//
		Wish_Get_UI()->Root.Add(dbgWnd);

		//
		m_bInit = true;
	}

	void Wish_Renderer_SetWorldMatrix(const mat4& other)
	{
		wish_renderer& renderer = Wish_Engine_GetContext()->Renderer;
		renderer.WorldMatrix = other;
	}

	const mat4& Wish_Renderer_GetWorldMatrix() {
		wish_renderer& renderer = Wish_Engine_GetContext()->Renderer;
		return renderer.WorldMatrix;
	}



	void PreLightingPasses()
	{
		wish_renderer& renderer = Wish_Engine_GetContext()->Renderer;
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
	void wish_renderer::Flush() {
		Wish_Timer_Start(&m_Timer_Flush);
		if (DeferredRendering) {

			//GBuffer this one is fairly simple!
			Wish_Timer_Start(&m_Timer_GBuffer);
			{
				Wish_DeferredRenderer_GeometryPass();
			}
			Wish_Timer_Stop(&m_Timer_GBuffer);

			Wish_Timer_Start(&m_Timer_PostProcessing);
			{
				//Post Processing

				//Create SSAO Image from GBuffer

				//Apply SSAO in lighting calculations using SSAO sampling?

				//Bind our Gbuffer for reading
				PreLightingPasses();

				Wish_DeferredRenderer_LightingPass();

				PostLightingPasses();

			}
			Wish_Timer_Stop(&m_Timer_PostProcessing);

			//Bind 0 for both read and write
			Wish_Framebuffer_BindWrite(NULL);
			Wish_Framebuffer_BindRead(NULL);

			//If we are debugging
			if (ShowTextures) {
				Wish_DeferredRenderer_DrawDebugTextures();
			}
		}
		else {
			Wish_Renderer_SetShaderProgram(SimpleShader);
			{
				// Turn on wireframe mode
				glDisable(GL_CULL_FACE);
				glEnable(GL_DEPTH_TEST);
				glDepthMask(GL_TRUE);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				Wish_Renderer_ApplyUniforms(SimpleShader, nullptr, nullptr);

				//Process render queue
				for (size_t i = 0; i < NumGeometries; i++) {
					wish_geometry* pGameObject = GeometryPassQueue[i];
					{
						pGameObject->Draw();
					}
				}

				glDisable(GL_DEPTH_TEST);
				glDepthMask(GL_FALSE);

				//Render our sphere at some location
				//Should put it under the cubes
				WorldMatrix = glm::translate(mat4(1.0f), vec3(-5, -5, -5)) * glm::scale(mat4(1.0f), vec3(0.1f, 0.1f, 0.1f));
				Wish_Renderer_ApplyUniforms(SimpleShader, nullptr, nullptr);

				Rect->Draw();
			}
			Wish_Renderer_SetShaderProgram(NULL);
		}
		Wish_Timer_Stop(&m_Timer_Flush);

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
		glOrtho(0.0f, Width, Height, 0.0f, -1.0f, 1.0f);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		//Draw font thing here
		glColor3f(1.0f, 1.0f, 1.0f);
		Wish_Texture_Bind(NULL, GL_TEXTURE0);

		//Set our UI shader
		//Wish_Renderer_SetShaderProgram(Wish_Get_UI()->UIProgram);
		//Wish_Renderer_ApplyUniforms(Wish_Get_UI()->UIProgram, nullptr, nullptr);


		//drawThing();

		Wish_Get_UI()->DebugString((std::string("FPS: ") + std::to_string(Wish_Engine_GetFPS())).c_str(), 5, 15);
		Wish_Get_UI()->DebugString((std::string("Flush: ") + std::to_string(Wish_Timer_GetValue(&m_Timer_Flush))).c_str(), 5, 30);
		Wish_Get_UI()->DebugString((std::string("GBuffer: ") + std::to_string(Wish_Timer_GetValue(&m_Timer_GBuffer))).c_str(), 5, 45);
		Wish_Get_UI()->DebugString((std::string("PostProcessing: ") + std::to_string(Wish_Timer_GetValue(&m_Timer_PostProcessing))).c_str(), 5, 60);
		Wish_Get_UI()->DebugString((std::string("Geometry Objects: ") + std::to_string(NumGeometries)).c_str(), 5, 75);
		
		//Draw UI!
		Wish_Get_UI()->Draw();

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		//Clear all render queues, regardless
		NumGeometries = 0;
		NumLights = 0;
	}

}

wish_shader_program* Wish::Wish_Renderer_GetShaderProgram() {
	wish_renderer& renderer = Wish_Engine_GetContext()->Renderer;
	return renderer.CurrentShaderProgram;
}

void Wish::Wish_Renderer_SetShaderProgram(wish_shader_program* pShaderProgram) {
	wish_renderer& renderer = Wish_Engine_GetContext()->Renderer;
	renderer.CurrentShaderProgram = pShaderProgram;
	Wish_ShaderProgram_Bind(pShaderProgram);
}


wish_camera* Wish::Wish_Renderer_GetCamera() {
	wish_renderer& renderer = Wish_Engine_GetContext()->Renderer;
	return renderer.ActiveCamera;
}

void Wish::Wish_Renderer_SetCamera(wish_camera* camera) {
	wish_renderer& renderer = Wish_Engine_GetContext()->Renderer;
	renderer.ActiveCamera = camera;
}

GLuint Wish::Wish_Renderer_GetWidth() {
	wish_renderer& renderer = Wish_Engine_GetContext()->Renderer;
	return renderer.Width;
}

GLuint Wish::Wish_Renderer_GetHeight() {
	wish_renderer& renderer = Wish_Engine_GetContext()->Renderer;
	return renderer.Height;
}

namespace Wish
{
	void Wish_Renderer_Submit(wish_geometry* geometry) {
		wish_renderer& renderer = Wish_Engine_GetContext()->Renderer;
		renderer.GeometryPassQueue[renderer.NumGeometries++] = geometry;
	}
	void Wish_Renderer_Submit(wish_light* light) {
		wish_renderer& renderer = Wish_Engine_GetContext()->Renderer;
		renderer.LighPassQueue[renderer.NumLights++] = light;
	}
}

void Wish::Wish_Renderer_BindTexture(GLuint slot, wish_texture* pTexture) {
	wish_renderer& renderer = Wish_Engine_GetContext()->Renderer;
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

//TODO(Meanzie): Revise
void Wish::Wish_Renderer_DrawMesh(wish_mesh* mesh, wish_material* material) {
	//Ugh, but the renderer decides the shader :o

	//Apply material
	Wish_Renderer_ApplyMaterial(Wish_Renderer_GetShaderProgram(), material);
	mesh->Draw();
}