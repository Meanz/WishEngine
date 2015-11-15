namespace Wish
{

	void Wish_DeferredRenderer_Init(geometry_buffer* geometryBuffer, u32 width, u32 height)
	{
		//Set up textures
		Wish_Framebuffer_Create(&geometryBuffer->Framebuffer);

		Wish_Texture_Create(&geometryBuffer->Diffuse, width, height, PixelFormat::RGBA32F, PixelFormat::RGB, GL_FLOAT);
		Wish_Framebuffer_AttachTexture(&geometryBuffer->Framebuffer, &geometryBuffer->Diffuse, FramebufferAttachment::COLOR0);

		Wish_Texture_Create(&geometryBuffer->Positions, width, height, PixelFormat::RGBA32F, PixelFormat::RGB, GL_FLOAT, GL_NEAREST, GL_NEAREST);
		Wish_Framebuffer_AttachTexture(&geometryBuffer->Framebuffer, &geometryBuffer->Positions, FramebufferAttachment::COLOR1);

		Wish_Texture_Create(&geometryBuffer->Normals, width, height, PixelFormat::RGBA32F, PixelFormat::RGB, GL_FLOAT);
		Wish_Framebuffer_AttachTexture(&geometryBuffer->Framebuffer, &geometryBuffer->Normals, FramebufferAttachment::COLOR2);

		Wish_Texture_Create(&geometryBuffer->Depth, width, height, PixelFormat::DEPTH_COMPONENT32F, PixelFormat::DEPTH_COMPONENT, GL_FLOAT, GL_LINEAR, GL_LINEAR);
		Wish_Framebuffer_AttachTexture(&geometryBuffer->Framebuffer, &geometryBuffer->Depth, FramebufferAttachment::DEPTH);

		Wish_Framebuffer_Validate(&geometryBuffer->Framebuffer);
		Wish_Framebuffer_Bind(NULL);
	}

	void Wish_DeferredRenderer_Stuff()
	{

	}

	void Wish_DeferredRenderer_GeometryPass()
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
		Wish_Renderer_SetShaderProgram(renderer.DeferredShader);
		Wish_Renderer_ApplyUniforms(renderer.DeferredShader, nullptr, nullptr);

		//Scene rendering
		for (size_t i = 0; i < renderer.NumGeometries; i++) {
			wish_geometry* pGameObject = renderer.GeometryPassQueue[i];
			{
				pGameObject->Draw();
			}
		}

		//Reset state
		glDepthMask(GL_FALSE);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
	}



	float CalcPointLightBSphere(wish_point_light* light)
	{
		float MaxChannel = fmax(fmax(light->Diffuse.x, light->Diffuse.y), light->Diffuse.z);

		float ret = (-light->AttenuationLinear + sqrtf(light->AttenuationLinear * light->AttenuationLinear - 4 * light->AttenuationExp * (light->AttenuationExp - 256 * MaxChannel * light->DiffuseIntensity)))
			/
			2 * light->AttenuationExp;

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

		Wish_Renderer_SetShaderProgram(renderer.NullShader);
		{
			//Apply uniforms
			Wish_Renderer_ApplyUniforms(renderer.PointLightShader, nullptr, light);

			//Draw light
			renderer.Sphere->Draw();
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
			Wish_Renderer_ApplyUniforms(Wish_Renderer_GetShaderProgram(), nullptr, pLight);

			//Draw light
			renderer.Sphere->Draw();

			glCullFace(GL_BACK);
			glDisable(GL_CULL_FACE);
		}
	}

	void Wish_DeferredRenderer_LightingPass()
	{
		wish_renderer_context& renderer = Wish_Engine_GetContext()->Renderer;
		//## BeginLightPasses();

		//Unbind all textures
		//And why are we doing this?
		for (u32 i = 0; i < MAX_TEXTURES; i++)
		{
			Wish_Renderer_BindTexture(i, NULL);
		}

		//Set's the framebuffer to the window buffer
		Wish_Framebuffer_BindRead(&renderer.GeometryBuffer.Framebuffer);
		Wish_Framebuffer_BindWrite(0);

		//Depth masks are off?
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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
				if (pLight->LightType == LightType::LIGHT_POINT) {
					float scalefactor = CalcPointLightBSphere((wish_point_light*)pLight);
					//Fake position
					renderer.WorldMatrix = glm::translate(mat4(1.0f), pLight->Position) * glm::scale(mat4(1.0f), vec3(scalefactor * 8));
					StencilPass((wish_point_light*)pLight);
					PointLightPass((wish_point_light*)pLight);
				}
			}
		}
		Wish_Renderer_SetShaderProgram(NULL);

		//## DSDirectionaLightPass();
		Wish_Renderer_SetShaderProgram(renderer.DirectionalLightShader);
		{
			for (size_t i = 0; i < renderer.NumLights; i++) {
				wish_light* pLight = renderer.LighPassQueue[i];
				{
					if (pLight->LightType == LightType::LIGHT_DIRECTIONAL) {
						Wish_Renderer_ApplyUniforms(renderer.DirectionalLightShader, nullptr, pLight);

						//Draw light
						renderer.Rect->Draw();
					}
				}
			}
		}
		Wish_Renderer_SetShaderProgram(NULL);


		//Meep
		glDisable(GL_BLEND);
	}

	void Wish_DeferredRenderer_DrawDebugTextures()
	{
		wish_renderer_context& renderer = Wish_Engine_GetContext()->Renderer;
		Wish_Renderer_SetShaderProgram(renderer.SimpleUnlitShader);
		{
			//We need to set the world matrix
			//Draw normal map
			Wish_Renderer_BindTexture(0, &renderer.GeometryBuffer.Normals);

			//Normals
			renderer.WorldMatrix = glm::translate(mat4(1.0f), vec3(0.75f, 0.75f, 0.0f)) * glm::scale(mat4(1.0f), vec3(0.25f, 0.25f, 0.25f));
			Wish_Renderer_ApplyUniforms(renderer.SimpleUnlitShader, nullptr, nullptr);
			renderer.WorldMatrix = (mat4(1.0f));

			renderer.Rect->Draw();

			//We need to set the world matrix
			//Draw normal map
			Wish_Renderer_BindTexture(0, &renderer.GeometryBuffer.Depth);

			//Normals
			renderer.WorldMatrix = glm::translate(mat4(1.0f), vec3(0.75f, 0.25f, 0.0f)) * glm::scale(mat4(1.0f), vec3(0.25f, 0.25f, 0.25f));
			Wish_Renderer_ApplyUniforms(renderer.SimpleUnlitShader, nullptr, nullptr);
			renderer.WorldMatrix = (mat4(1.0f));

			renderer.Rect->Draw();

			//SSAO
			Wish_Renderer_BindTexture(0, &renderer.m_SSAOTexture.Texture);

			renderer.WorldMatrix = glm::translate(mat4(1.0f), vec3(0.75f, -0.25f, 0.0f)) * glm::scale(mat4(1.0f), vec3(0.25f, 0.25f, 0.25f));
			Wish_Renderer_ApplyUniforms(renderer.SimpleUnlitShader, nullptr, nullptr);
			renderer.WorldMatrix = (mat4(1.0f));

			renderer.Rect->Draw();

			//SSAO Blurred
			Wish_Renderer_BindTexture(0, &renderer.m_SSAOBlurredTexture.Texture);

			renderer.WorldMatrix = glm::translate(mat4(1.0f), vec3(0.75f, -0.75f, 0.0f)) * glm::scale(mat4(1.0f), vec3(0.25f, 0.25f, 0.25f));
			Wish_Renderer_ApplyUniforms(renderer.SimpleUnlitShader, nullptr, nullptr);
			renderer.WorldMatrix = (mat4(1.0f));

			renderer.Rect->Draw();
		}
		Wish_Renderer_SetShaderProgram(NULL);
	}
}