namespace Wish
{
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

	void Wish_Renderer_ApplyMaterial(wish_shader_program* program, wish_material* material) {
		wish_renderer_context& renderer = Wish_Engine_GetContext()->Renderer;
		if (material != NULL) {

			//Static albedo ? :o
			wish_texture* pAlbedo = material->Albedo;
			if (pAlbedo != NULL) {
				Wish_Renderer_BindTexture(0, pAlbedo);
			}

			//Iterate through all uniforms
			GLuint samplerIndex = 0;
			wish_shader_uniform_data& sud = program->UniformData;
			for (size_t i = 0; i < sud.NumUniforms; i++) {
				GLint loc = sud.Locations[i];
				wish_murmur3_hash& hash = sud.Hashes[i];
				if (loc == -1) {
					//Bad location
					continue;
				}
				const char* name = Wish_CString(sud.Names[i]);

				if (loc == -1) {
					//Bad location
					continue;
				}

				if (hash == hashTable[R_World])
				{
					glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(renderer.WorldMatrix));
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
		wish_shader_uniform_data& sud = pShaderProgram->UniformData;
		for (size_t i = 0; i < sud.NumUniforms; i++) {
			GLint loc = sud.Locations[i];
			wish_murmur3_hash& hash = sud.Hashes[i];
			if (loc == -1) {
				//Bad location
				continue;
			}
			const char* name = Wish_CString(sud.Names[i]);
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
				glUniform1i(loc, renderer.Width);
			}
			else if (hash == hashTable[R_Height]) {
				glUniform1i(loc, renderer.Height);
			}
			//Access something from the rendering engine
			else if (hash == hashTable[R_OrthoProjection]) {
				wish_camera camera;
				camera.Ortho(0.0f, (GLfloat)renderer.Width, (GLfloat)renderer.Height, 0.0f, -1.0f, 1.0f);
				camera.CalculateViewMatrix();
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
				glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(renderer.WorldMatrix));
			}
			else if (hash == hashTable[R_WVP]) {
				//Flat for now
				glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(Wish_Renderer_GetCamera()->Projection * Wish_Renderer_GetCamera()->View * renderer.WorldMatrix));
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
				glUniform3fv(loc, 1, value_ptr(pLight->Diffuse));
			}
			else if (hash == hashTable[L_DiffuseIntensity])
			{
				if (pLight == nullptr) continue;
				glUniform1f(loc, pLight->DiffuseIntensity);
			}
			else if (hash == hashTable[L_Specular])
			{
				if (pLight == nullptr) continue;
				glUniform3fv(loc, 1, value_ptr(pLight->Specular));
			}
			else if (hash == hashTable[L_SpecularIntensity])
			{
				if (pLight == nullptr) continue;
				glUniform1f(loc, pLight->SpecularIntensity);
			}
			else if (hash == hashTable[L_AttenuationConstant])
			{
				if (pLight == nullptr) continue;
				glUniform1f(loc, ((wish_point_light*)pLight)->AttenuationConstant);
			}
			else if (hash == hashTable[L_AttenuationLinear])
			{
				if (pLight == nullptr) continue;
				glUniform1f(loc, ((wish_point_light*)pLight)->AttenuationLinear);
			}
			else if (hash == hashTable[L_AttenuationExp])
			{
				if (pLight == nullptr) continue;
				glUniform1f(loc, ((wish_point_light*)pLight)->AttenuationExp);
			}
			else  {
				printf("Unhandled uniform %s\n", name);
			}

		}
	}
};