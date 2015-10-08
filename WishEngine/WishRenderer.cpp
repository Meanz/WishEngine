#include "stdafx.h"
#include "Wish.hpp"
#include "WishRenderer.hpp"
#include "WishPrimitives.h"
#include "WishOBJLoader.hpp"
#include "WishUniforms.cpp"

FreetypeFont m_DebugFont;


static wish_murmur3_hash hashTable[] = {
	Wish_Hash("L_Position"),				//0
	Wish_Hash("L_Diffuse"),					//1
	Wish_Hash("L_DiffuseIntensity"),		//2
	Wish_Hash("L_Specular"),				//3
	Wish_Hash("L_SpecularIntensity"),		//4
	Wish_Hash("L_AttenuationConstant"),		//5
	Wish_Hash("L_AttenuationLinear"),		//6
	Wish_Hash("L_AttenuationExp"),			//7
	Wish_Hash("R_World"),					//8
	Wish_Hash("R_View"),					//9
	Wish_Hash("R_Projection"),				//10
	Wish_Hash("R_WVP"),						//11
	Wish_Hash("R_Normal"),					//12
	Wish_Hash("R_OrthoProjection"),			//13
	Wish_Hash("R_Kernel"),					//14
	Wish_Hash("T_Albedo"),					//15
	Wish_Hash("T_Diffuse"),					//16
	Wish_Hash("T_Position"),				//17
	Wish_Hash("T_Normal"),					//18
	Wish_Hash("T_SSAO"),					//19
	Wish_Hash("R_Width"),					//20
	Wish_Hash("R_Height")					//21
};

//Lighting
#define L_Position 0
#define L_Diffuse 1
#define L_DiffuseIntensity 2
#define L_Specular 3
#define L_SpecularIntensity 4
#define L_AttenuationConstant 5
#define L_AttenuationLinear 6
#define L_AttenuationExp 7

//M Values

//T Values
#define T_Albedo 15
#define T_Diffuse 16
#define T_Position 17
#define T_Normal 18
#define T_SSAO	19

//R Values
#define R_World 8
#define R_View 9
#define R_Projection 10
#define R_WVP 11
#define R_Normal 12
#define R_OrthoProjection 13
#define R_Kernel 14
#define R_Width 20
#define R_Height 21

void Wish::Wish_Renderer_ApplyMaterial(wish_shader_program* pShaderProgram, wish_material* pMaterial) {

	if (pMaterial != NULL) {

		//Static albedo ? :o
		wish_texture* pAlbedo = pMaterial->Albedo;
		if (pAlbedo != NULL) {
			Wish_Renderer_BindTexture(0, pAlbedo);
		}

		//Set world matrix
		mat4& worldMatrix = *pMaterial->World;
		//Wish_Material_Get(pMaterial, "World", &worldMatrix);

		//Iterate through all uniforms
		GLuint samplerIndex = 0;
		wish_shader_uniform_data& sud = pShaderProgram->m_UniformData;
		for (size_t i = 0; i < sud.m_vUniforms.size(); i++) {
			GLint loc = sud.m_vUniformLocations[i];
			wish_murmur3_hash& hash = sud.m_vHashedUniforms[i];
			if (loc == -1) {
				//Bad location
				continue;
			}
			String& name = sud.m_vUniforms[i];

			if (loc == -1) {
				//Bad location
				continue;
			}

			if (hash == hashTable[R_World])
			{
				glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(worldMatrix));
			}
		}
	}

}

void Wish_Renderer_ApplyUniforms(wish_shader_program* pShaderProgram, wish_material* pMaterial, wish_light* pLight) {
	wish_renderer_context& renderer = Wish_Engine_GetContext()->Renderer;
	if (pShaderProgram == NULL) {
		return;
	}

	if (!renderer.isKernelDefined) {

		//val = 2 * (0-1) - 1.0;

		for (u32 i = 0; i < KERNEL_SIZE; i++) {
			float scale = (float)i / (float)(KERNEL_SIZE);
			vec3 v;
			v.x = 2.0f * (float)rand() / RAND_MAX - 1.0f;
			v.y = 2.0f * (float)rand() / RAND_MAX - 1.0f;
			v.z = 2.0f * (float)rand() / RAND_MAX - 1.0f;
			// Use an acceleration function so more points are
			// located closer to the origin
			v *= (0.1f + 0.9f * scale * scale);

			renderer.kernel[i] = v;
		}

		renderer.isKernelDefined = true;
	}

	//Iterate through all uniforms
	GLuint samplerIndex = 0;
	wish_shader_uniform_data& sud = pShaderProgram->m_UniformData;
	for (size_t i = 0; i < sud.m_vUniforms.size(); i++) {
		GLint loc = sud.m_vUniformLocations[i];
		wish_murmur3_hash& hash = sud.m_vHashedUniforms[i];
		if (loc == -1) {
			//Bad location
			continue;
		}
		String& name = sud.m_vUniforms[i];
		//Hardcode for now
		if (hash == hashTable[T_Albedo])
		{
			GLuint texLoc = (samplerIndex++);
			glUniform1i(loc, (samplerIndex - 1));
		}
		else if (hash == hashTable[T_Diffuse]) {
			GLuint texLoc = (samplerIndex++);
			glUniform1i(loc, texLoc);
			Wish_Renderer_BindTexture(texLoc, &renderer.GeometryBuffer.Diffuse);
		}
		else if (hash == hashTable[T_Position]) {
			GLuint texLoc = (samplerIndex++);
			glUniform1i(loc, texLoc);
			Wish_Renderer_BindTexture(texLoc, &renderer.GeometryBuffer.Positions);
		}
		else if (hash == hashTable[T_Normal]) {
			GLuint texLoc = (samplerIndex++);
			glUniform1i(loc, texLoc);
			Wish_Renderer_BindTexture(texLoc, &renderer.GeometryBuffer.Normals);
		}
		else if (hash == hashTable[T_SSAO]) {
			GLuint texLoc = (samplerIndex++);
			glUniform1i(loc, texLoc);
			Wish_Renderer_BindTexture(texLoc, &renderer.m_SSAOTexture.Texture);
		}
		else if (hash == hashTable[R_Width]) {
			glUniform1i(loc, renderer.m_Width);
		}
		else if (hash == hashTable[R_Height]) {
			glUniform1i(loc, renderer.m_Height);
		}
		//Access something from the rendering engine
		else if (hash == hashTable[R_OrthoProjection]) {
			wish_camera camera;
			ZeroMemory(&camera, sizeof(wish_camera));
			Wish_Transform_Init(&camera.Transform);

			Wish_Camera_Ortho2D(&camera, 0.0f, (GLfloat)renderer.m_Width, (GLfloat)renderer.m_Height, 0.0f, -1.0f, 1.0f);
			Wish_Camera_CalculateViewMatrix(&camera);
			glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(camera.Projection));
		}
		else if (hash == hashTable[R_Projection]) {
			glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(Wish_Renderer_GetCamera()->Projection));
		}
		else if (name == "R_Projection2") {
			glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(Wish_Renderer_GetCamera()->Projection));
		}
		else if (hash == hashTable[R_View]) {
			glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(Wish_Renderer_GetCamera()->View));
		}
		else if (hash == hashTable[R_World]) {
			//Flat for now
			glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(renderer.m_WorldMatrix));
		}
		else if (hash == hashTable[R_WVP]) {
			//Flat for now
			glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(Wish_Renderer_GetCamera()->Projection * Wish_Renderer_GetCamera()->View * renderer.m_WorldMatrix));
		}
		else if (hash == hashTable[R_Normal]) {
			//Flat for now
			glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(mat4(1.0f)));
		}
		else if (hash == hashTable[R_Kernel]) {
			glUniform3fv(loc, KERNEL_SIZE, value_ptr(renderer.kernel[0]));
		}
		else if (hash == hashTable[L_Position])
		{
			//The position of the light?
			if (pLight == nullptr) continue;
			glUniform3fv(loc, 1, value_ptr(pLight->Position));
		}
		else if (hash == hashTable[L_Diffuse])
		{
			if (pLight == nullptr) continue;
			glUniform3fv(loc, 1, value_ptr(pLight->m_Diffuse));
		}
		else if (hash == hashTable[L_DiffuseIntensity])
		{
			if (pLight == nullptr) continue;
			glUniform1f(loc, pLight->m_DiffuseIntensity);
		}
		else if (hash == hashTable[L_Specular])
		{
			if (pLight == nullptr) continue;
			glUniform3fv(loc, 1, value_ptr(pLight->m_Specular));
		}
		else if (hash == hashTable[L_SpecularIntensity])
		{
			if (pLight == nullptr) continue;
			glUniform1f(loc, pLight->m_SpecularIntensity);
		}
		else if (hash == hashTable[L_AttenuationConstant])
		{
			if (pLight == nullptr) continue;
			glUniform1f(loc, ((wish_point_light*)pLight)->m_AttenuationConstant);
		}
		else if (hash == hashTable[L_AttenuationLinear])
		{
			if (pLight == nullptr) continue;
			glUniform1f(loc, ((wish_point_light*)pLight)->m_AttenuationLinear);
		}
		else if (hash == hashTable[L_AttenuationExp])
		{
			if (pLight == nullptr) continue;
			glUniform1f(loc, ((wish_point_light*)pLight)->m_AttenuationExp);
		}
		else  {
			printf("Unhandled uniform %s\n", name.c_str());
		}

	}
}

void Wish::Wish_RenderTexture_Create(wish_render_texture* renderTexture, GLuint width, GLuint height)
{
	Wish_Texture_Create(&renderTexture->Texture, width, height, PixelFormat::RGBA32F, PixelFormat::RGBA, GL_FLOAT, GL_NEAREST, GL_NEAREST);
	Wish_Framebuffer_Create(&renderTexture->Framebuffer);
	Wish_Framebuffer_AttachTexture(&renderTexture->Framebuffer, &renderTexture->Texture, FramebufferAttachment::COLOR0);
	Wish_Framebuffer_Validate(&renderTexture->Framebuffer);
}

void Wish::Wish_RenderTexture_Prepare(wish_render_texture* renderTexture)
{
	Wish_Framebuffer_BindWrite(&renderTexture->Framebuffer);
	//GLenum buffers[] = { FramebufferAttachment::COLOR0 };
	//glDrawBuffer(FramebufferAttachment::COLOR0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Wish::Wish_Renderer_Init(int width, int height)
{
	wish_renderer_context& renderer = Wish_Engine_GetContext()->Renderer;
	renderer.m_activeCamera = NULL;
	renderer.m_DeferredRendering = true;
	renderer.ShowTextures = true;
	renderer.m_WorldMatrix = mat4(1.0f);

	renderer.m_Width = width;
	renderer.m_Height = height;

	//Set up textures
	Wish_Framebuffer_Create(&renderer.GeometryBuffer.Framebuffer);

	Wish_Texture_Create(&renderer.GeometryBuffer.Diffuse, width, height, PixelFormat::RGBA32F, PixelFormat::RGB, GL_FLOAT);
	Wish_Framebuffer_AttachTexture(&renderer.GeometryBuffer.Framebuffer, &renderer.GeometryBuffer.Diffuse, FramebufferAttachment::COLOR0);

	Wish_Texture_Create(&renderer.GeometryBuffer.Positions, width, height, PixelFormat::RGBA32F, PixelFormat::RGB, GL_FLOAT, GL_NEAREST, GL_NEAREST);
	Wish_Framebuffer_AttachTexture(&renderer.GeometryBuffer.Framebuffer, &renderer.GeometryBuffer.Positions, FramebufferAttachment::COLOR1);

	Wish_Texture_Create(&renderer.GeometryBuffer.Normals, width, height, PixelFormat::RGBA32F, PixelFormat::RGB, GL_FLOAT);
	Wish_Framebuffer_AttachTexture(&renderer.GeometryBuffer.Framebuffer, &renderer.GeometryBuffer.Normals, FramebufferAttachment::COLOR2);

	Wish_Texture_Create(&renderer.GeometryBuffer.Depth, width, height, PixelFormat::DEPTH_COMPONENT32F, PixelFormat::DEPTH_COMPONENT, GL_FLOAT, GL_LINEAR, GL_LINEAR);
	Wish_Framebuffer_AttachTexture(&renderer.GeometryBuffer.Framebuffer, &renderer.GeometryBuffer.Depth, FramebufferAttachment::DEPTH);

	Wish_Framebuffer_Validate(&renderer.GeometryBuffer.Framebuffer);

	Wish_RenderTexture_Create(&renderer.m_SSAOTexture, width, height);
	Wish_RenderTexture_Create(&renderer.m_SSAOBlurredTexture, width, height);

	//Wish_RenderTexture_Create(&renderer.m_OffScreenA, width, height);
	//Wish_RenderTexture_Create(&renderer.m_OffScreenB, width, height);

	Wish_Framebuffer_Bind(NULL);

	//Let's populate our rect
	renderer.m_Rect = Wish_Primitive_Quad(-1, 1, -1, 1, 1, 1);

	renderer.m_pDeferredShader = Wish_Asset_LoadShader("deferredShading", "./data/shaders/deferredShading.glsl");
	renderer.m_pDeferredRenderingShader = Wish_Asset_LoadShader("deferredRendering", "./data/shaders/deferredRendering.glsl");

	renderer.m_pSimpleShader = Wish_Asset_LoadShader("simple", "./data/shaders/simple.glsl");
	renderer.m_pSimpleUnlitShader = Wish_Asset_LoadShader("simple_unlit", "./data/shaders/unlit_simple.glsl");

	renderer.m_pSSAOShader = Wish_Asset_LoadShader("ssao", "./data/shaders/ssao.glsl");
	renderer.m_pBlurShader = Wish_Asset_LoadShader("blur", "./data/shaders/blur.glsl");

	renderer.m_pPointLightShader = Wish_Asset_LoadShader("point_light", "./data/shaders/pointlight.glsl");
	renderer.m_pDirectionalLightShader = Wish_Asset_LoadShader("directional_light", "./data/shaders/directionallight.glsl");

	renderer.m_pNullShader = Wish_Asset_LoadShader("null", "./data/shaders/nulltech.glsl");

	m_DebugFont.LoadFont("./data/fonts/arial.ttf", 14);
	renderer.m_Sphere = WishOBJLoader_Load("./data/models/sphere.obj");


	renderer.m_bInit = true;
}

const mat4& Wish::Wish_Renderer_GetWorldMatrix() {
	wish_renderer_context& renderer = Wish_Engine_GetContext()->Renderer;
	return renderer.m_WorldMatrix;
}

wish_shader_program* Wish::Wish_Renderer_GetShaderProgram() {
	wish_renderer_context& renderer = Wish_Engine_GetContext()->Renderer;
	return renderer.m_pCurrentShaderProgram;
}

void Wish::Wish_Renderer_SetShaderProgram(wish_shader_program* pShaderProgram) {
	wish_renderer_context& renderer = Wish_Engine_GetContext()->Renderer;
	renderer.m_pCurrentShaderProgram = pShaderProgram;
	if (pShaderProgram != NULL && pShaderProgram->glHandle > 0) {
		Wish_ShaderProgram_Bind(pShaderProgram);
	}
	else {
		glUseProgram(0);
	}
}


wish_camera* Wish::Wish_Renderer_GetCamera() {
	wish_renderer_context& renderer = Wish_Engine_GetContext()->Renderer;
	return renderer.m_activeCamera;
}

void Wish::Wish_Renderer_SetCamera(wish_camera* camera) {
	wish_renderer_context& renderer = Wish_Engine_GetContext()->Renderer;
	renderer.m_activeCamera = camera;
}

GLuint Wish::Wish_Renderer_GetWidth() {
	wish_renderer_context& renderer = Wish_Engine_GetContext()->Renderer;
	return renderer.m_Width;
}

GLuint Wish::Wish_Renderer_GetHeight() {
	wish_renderer_context& renderer = Wish_Engine_GetContext()->Renderer;
	return renderer.m_Height;
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

void GeometryPass()
{
	wish_renderer_context& renderer = Wish_Engine_GetContext()->Renderer;
	//Active things
	glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);

	//Swap FrameBuffer to our deferred shading one!
	Wish_Framebuffer_Bind(&renderer.GeometryBuffer.Framebuffer);

	//Clear buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//Set draw buffers
	GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, buffers);

	//Activate deferred shader
	Wish_Renderer_SetShaderProgram(renderer.m_pDeferredShader);

	Wish_Renderer_ApplyUniforms(renderer.m_pDeferredShader, nullptr, nullptr);

	//Scene rendering
	for (size_t i = 0; i < renderer.NumGeometries; i++) {
		wish_geometry* pGameObject = renderer.GeometryPassQueue[i];
		{
			Wish_Geometry_Draw(pGameObject);
		}
	}

	//Reset state
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}

void PreLightingPasses()
{
	wish_renderer_context& renderer = Wish_Engine_GetContext()->Renderer;
	//Wish_RenderTexture_Prepare(&renderer.m_SSAOTexture);

	Wish_Framebuffer_BindRead(&renderer.GeometryBuffer.Framebuffer);
	Wish_Framebuffer_BindWrite(&renderer.m_SSAOTexture.Framebuffer);
	glClear(GL_COLOR_BUFFER_BIT);

	Wish_Renderer_SetShaderProgram(renderer.m_pSSAOShader);
	Wish_Renderer_ApplyUniforms(renderer.m_pSSAOShader, nullptr, nullptr);
	Wish_Mesh_Draw(renderer.m_Rect);
}


float CalcPointLightBSphere(wish_point_light* light)
{
	float MaxChannel = fmax(fmax(light->base.m_Diffuse.x, light->base.m_Diffuse.y), light->base.m_Diffuse.z);

	float ret = (-light->m_AttenuationLinear + sqrtf(light->m_AttenuationLinear * light->m_AttenuationLinear - 4 * light->m_AttenuationExp * (light->m_AttenuationExp - 256 * MaxChannel * light->base.m_DiffuseIntensity)))
		/
		2 * light->m_AttenuationExp;

	return ret;
}

void StencilPass(wish_point_light* light) {
	wish_renderer_context& renderer = Wish_Engine_GetContext()->Renderer;
	glEnable(GL_STENCIL_TEST);
	glClear(GL_STENCIL_BUFFER_BIT);

	//Shader null tech wut

	glStencilFunc(GL_ALWAYS, 0, 0);

	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

	Wish_Renderer_SetShaderProgram(renderer.m_pNullShader);
	{
		//Apply uniforms
		Wish_Renderer_ApplyUniforms(renderer.m_pPointLightShader, nullptr, &light->base);

		//Draw light
		Wish_Mesh_Draw(renderer.m_Sphere);
	}
	Wish_Renderer_SetShaderProgram(NULL);

	glDisable(GL_STENCIL_TEST);
}

void PointLightPass(wish_point_light* pLight) {
	//Point light test
	wish_renderer_context& renderer = Wish_Engine_GetContext()->Renderer;
	{
		glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

		//Disable depth test
		glDisable(GL_DEPTH_TEST);

		//Setup additive blending
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		//Don't render front face ? =O
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		//
		Wish_Renderer_ApplyUniforms(Wish_Renderer_GetShaderProgram(), nullptr, &pLight->base);

		//Draw light
		Wish_Mesh_Draw(renderer.m_Sphere);

		glCullFace(GL_BACK);
		glDisable(GL_CULL_FACE);
	}


}

void Wish_Renderer_UnbindAll()
{
	for (u32 i = 0; i < MAX_TEXTURES; i++)
	{
		Wish_Renderer_BindTexture(i, NULL);
	}
}

void LightingPass()
{
	wish_renderer_context& renderer = Wish_Engine_GetContext()->Renderer;
	//## BeginLightPasses();

	//Unbind all textures
	Wish_Renderer_UnbindAll();

	//Set's the framebuffer to the window buffer
	Wish_Framebuffer_BindRead(&renderer.GeometryBuffer.Framebuffer);
	Wish_Framebuffer_BindWrite(0);

	//glNamedFramebufferDrawBuffers();

	//Wish_Framebuffer_BindRead(&renderer.GeometryBuffer.Framebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//Setup lighting passes
	//glEnable(GL_BLEND);
	//glBlendEquation(GL_FUNC_ADD);
	//glBlendFunc(GL_ONE, GL_ONE);

	//## DSPointLightPass();

	//Apply uniforms


	//Wish_Renderer_SetShaderProgram(renderer.m_pPointLightShader);
	//Apply uniforms
	//Wish_Renderer_ApplyUniforms(renderer.m_pPointLightShader, nullptr, nullptr);

	for (size_t i = 0; i < renderer.NumLights; i++) {
		wish_light* pLight = renderer.LighPassQueue[i];
		{
			if (pLight->m_LightType == LightType::LIGHT_POINT) {
				float scalefactor = CalcPointLightBSphere((wish_point_light*)pLight);
				//Fake position
				renderer.m_WorldMatrix = glm::translate(mat4(1.0f), pLight->Position) * glm::scale(mat4(1.0f), vec3(scalefactor * 8));
				StencilPass((wish_point_light*)pLight);
				PointLightPass((wish_point_light*)pLight);
			}
		}
	}
	Wish_Renderer_SetShaderProgram(NULL);

	//## DSDirectionaLightPass();
	Wish_Renderer_SetShaderProgram(renderer.m_pDirectionalLightShader);
	{
		for (size_t i = 0; i < renderer.NumLights; i++) {
			wish_light* pLight = renderer.LighPassQueue[i];
			{
				if (pLight->m_LightType == LightType::LIGHT_DIRECTIONAL) {
					Wish_Renderer_ApplyUniforms(renderer.m_pDirectionalLightShader, nullptr, pLight);

					//Draw light
					Wish_Mesh_Draw(renderer.m_Rect);
				}
			}
		}
	}
	Wish_Renderer_SetShaderProgram(NULL);


	//Meep
	glDisable(GL_BLEND);
}

void PostLightingPasses()
{
	/*//Draw gbuffer contents
	Framebuffer::Bind(0);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//Draw the last quad!
	SetShaderProgram(m_pDeferredRenderingShader);
	{
	Texture::Enable();
	GeometryBuffer.Diffuse.Bind(GL_TEXTURE0);
	GeometryBuffer.Positions.Bind(GL_TEXTURE1);
	GeometryBuffer.Normals.Bind(GL_TEXTURE2);
	m_SSAOBlurredTexture.GetTexture().Bind(GL_TEXTURE3);

	ApplyUniforms(m_pDeferredRenderingShader, this, nullptr);
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
	if (renderer.m_DeferredRendering) {

		//GBuffer this one is fairly simple!
		Wish_Timer_Start(&renderer.m_Timer_GBuffer);
		{
			GeometryPass();
		}
		Wish_Timer_Stop(&renderer.m_Timer_GBuffer);

		Wish_Timer_Start(&renderer.m_Timer_PostProcessing);
		{
			//Post Processing

			//Create SSAO Image from GBuffer

			//Apply SSAO in lighting calculations using SSAO sampling?

			//Bind our Gbuffer for reading

			PreLightingPasses();
			LightingPass();
			PostLightingPasses();

			/*
			Wish_Renderer_SetShaderProgram(renderer.m_pSimpleUnlitShader);
			Wish_Renderer_BindTexture(0, &renderer.GeometryBuffer.Normals);
			Wish_Texture_Enable();

			renderer.m_WorldMatrix = glm::translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)) * glm::scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
			Wish_Renderer_ApplyUniforms(renderer.m_pSimpleUnlitShader, nullptr, nullptr);
			renderer.m_WorldMatrix = (mat4(1.0f));
			Wish_Mesh_Draw(&renderer.m_Rect);
			Wish_Renderer_SetShaderProgram(NULL);*/

		}
		Wish_Timer_Stop(&renderer.m_Timer_PostProcessing);

		//Bind 0 for both read and write
		Wish_Framebuffer_BindWrite(NULL);
		Wish_Framebuffer_BindRead(NULL);

		//If we are debugging
		if (renderer.ShowTextures) {
			glDisable(GL_DEPTH_TEST);

			Wish_Renderer_SetShaderProgram(renderer.m_pSimpleUnlitShader);
			{
				//We need to set the world matrix
				//Draw normal map
				Wish_Renderer_BindTexture(0, &renderer.GeometryBuffer.Normals);

				//Normals
				renderer.m_WorldMatrix = glm::translate(mat4(1.0f), vec3(0.75f, 0.75f, 0.0f)) * glm::scale(mat4(1.0f), vec3(0.25f, 0.25f, 0.25f));
				Wish_Renderer_ApplyUniforms(renderer.m_pSimpleUnlitShader, nullptr, nullptr);
				renderer.m_WorldMatrix = (mat4(1.0f));

				Wish_Mesh_Draw(renderer.m_Rect);

				//We need to set the world matrix
				//Draw normal map
				Wish_Renderer_BindTexture(0, &renderer.GeometryBuffer.Depth);

				//Normals
				renderer.m_WorldMatrix = glm::translate(mat4(1.0f), vec3(0.75f, 0.25f, 0.0f)) * glm::scale(mat4(1.0f), vec3(0.25f, 0.25f, 0.25f));
				Wish_Renderer_ApplyUniforms(renderer.m_pSimpleUnlitShader, nullptr, nullptr);
				renderer.m_WorldMatrix = (mat4(1.0f));

				Wish_Mesh_Draw(renderer.m_Rect);

				//SSAO
				Wish_Renderer_BindTexture(0, &renderer.m_SSAOTexture.Texture);

				renderer.m_WorldMatrix = glm::translate(mat4(1.0f), vec3(0.75f, -0.25f, 0.0f)) * glm::scale(mat4(1.0f), vec3(0.25f, 0.25f, 0.25f));
				Wish_Renderer_ApplyUniforms(renderer.m_pSimpleUnlitShader, nullptr, nullptr);
				renderer.m_WorldMatrix = (mat4(1.0f));

				Wish_Mesh_Draw(renderer.m_Rect);

				//SSAO Blurred
				Wish_Renderer_BindTexture(0, &renderer.m_SSAOBlurredTexture.Texture);

				renderer.m_WorldMatrix = glm::translate(mat4(1.0f), vec3(0.75f, -0.75f, 0.0f)) * glm::scale(mat4(1.0f), vec3(0.25f, 0.25f, 0.25f));
				Wish_Renderer_ApplyUniforms(renderer.m_pSimpleUnlitShader, nullptr, nullptr);
				renderer.m_WorldMatrix = (mat4(1.0f));

				Wish_Mesh_Draw(renderer.m_Rect);
			}
			Wish_Renderer_SetShaderProgram(NULL);
		}
	}
	else {
		Wish_Renderer_SetShaderProgram(renderer.m_pSimpleShader);
		{
			//glEnable(GL_CULL_FACE);
			// Turn on wireframe mode
			//glPolygonMode(GL_FRONT, GL_LINE);
			//glPolygonMode(GL_BACK, GL_LINE);

			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_TRUE);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			Wish_Renderer_ApplyUniforms(renderer.m_pSimpleShader, nullptr, nullptr);

			//Process render queue
			for (size_t i = 0; i < renderer.NumGeometries; i++) {
				wish_geometry* pGameObject = renderer.GeometryPassQueue[i];
				{
					Wish_Geometry_Draw(pGameObject);
				}
			}

			//Render our sphere at some location
			//Should put it under the cubes
			renderer.m_WorldMatrix = glm::translate(mat4(1.0f), vec3(-5, -5, -5)) * glm::scale(mat4(1.0f), vec3(0.1f, 0.1f, 0.1f));
			Wish_Renderer_ApplyUniforms(renderer.m_pSimpleShader, nullptr, nullptr);

			Wish_Mesh_Draw(renderer.m_Rect);

			//glPolygonMode(GL_FRONT, GL_FILL);
			//glPolygonMode(GL_BACK, GL_FILL);
		}
		Wish_Renderer_SetShaderProgram(NULL);
	}
	Wish_Timer_Stop(&renderer.m_Timer_Flush);

	//Legacy draw
	Wish_Framebuffer_Bind(NULL);
	Wish_Renderer_SetShaderProgram(NULL);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	//Disable other texture slots?
	Wish_Texture_Bind(NULL, GL_TEXTURE0);
	Wish_Texture_Bind(NULL, GL_TEXTURE1);
	Wish_Texture_Bind(NULL, GL_TEXTURE2);
	Wish_Texture_Bind(NULL, GL_TEXTURE3);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0f, renderer.m_Width, renderer.m_Height, 0.0f, -1.0f, 1.0f);
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

void Wish::Wish_Renderer_DrawMesh(wish_mesh* mesh, wish_material* material) {
	//Ugh, but the renderer decides the shader :o

	//Apply material
	Wish_Renderer_ApplyMaterial(Wish_Renderer_GetShaderProgram(), material);
	Wish_Mesh_Draw(mesh);
}