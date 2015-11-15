namespace Wish
{
	b32 Wish::Wish_Shader_Compile(wish_shader* shader)
	{
		b32 result = false;
		if (Wish_String_IsValid(shader->Source))
		{
			ASSERT((shader->ShaderType == GL_FRAGMENT_SHADER || shader->ShaderType == GL_VERTEX_SHADER));
			ASSERT((shader != NULL));

			if (shader->GLHandle) {
				//Delete handle if it exists
				glDeleteShader(shader->GLHandle);
			}

			shader->GLHandle = glCreateShader(shader->ShaderType);

			ASSERT((shader->GLHandle > 0));

			const GLchar* src = (const GLchar*)Wish_CString(shader->Source);

			glShaderSource(shader->GLHandle, 1, &src, 0); //Copy the string into a GLObject

			//perform gl compile
			glCompileShader(shader->GLHandle);

			//Perform error checking
			GLint isCompiled = 0;
			glGetShaderiv(shader->GLHandle, GL_COMPILE_STATUS, (int*)&isCompiled);

			if (isCompiled == GL_FALSE)
			{
				//Store error string!
				GLint maxLength = 0;
				glGetShaderiv(shader->GLHandle, GL_INFO_LOG_LENGTH, &maxLength);
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader->GLHandle, maxLength, &maxLength, &infoLog[0]);
				shader->ErrorString = (char*)&infoLog[0];
			}

			if (isCompiled == GL_TRUE)
			{
				result = true;
			}
		}
		return result;
	}

	b32 Wish_ShaderProgram_Link(wish_shader_program* program) {
		b32 result = false;
		if (program)
		{
			if (program->VertexShader.GLHandle && program->FragmentShader.GLHandle)
			{
				if (program->GLHandle) {
					glDeleteProgram(program->GLHandle);
				}
				program->GLHandle = glCreateProgram();

				//This shouldn't really happen, so yeah
				ASSERT(program->GLHandle > 0);

				//attach fragment and vertex
				glAttachShader(program->GLHandle, program->VertexShader.GLHandle);
				glAttachShader(program->GLHandle, program->FragmentShader.GLHandle);

				glLinkProgram(program->GLHandle);

				GLint isLinked = 0;
				glGetProgramiv(program->GLHandle, GL_LINK_STATUS, (int*)&isLinked);

				if (isLinked == GL_FALSE)
				{
					//Store error string!
					GLint maxLength = 0;
					glGetProgramiv(program->GLHandle, GL_INFO_LOG_LENGTH, &maxLength);
					std::vector<GLchar> infoLog(maxLength);
					glGetProgramInfoLog(program->GLHandle, maxLength, &maxLength, &infoLog[0]);
					program->ErrorString = (char*)&infoLog[0];
				}

				//detach
				glDetachShader(program->GLHandle, program->VertexShader.GLHandle);
				glDetachShader(program->GLHandle, program->FragmentShader.GLHandle);


				if (isLinked == GL_TRUE) {
					result = true;
				}
			}
			else
			{
				//How is this even allocated?
				//new string? WHERE????
				//We really need to find a place to alloc all our strings

				//I don't like this, who is responsible for cleanup?
				program->ErrorString = Wish_Stringf("Both VertexShader and FragmentShader must be attached to Program before linking. VS(%i) FS(%i)\n", program->VertexShader, program->FragmentShader);
			}
		}
		else
		{
			//This is really bad for business
			ASSERT(0);
		}
		return result;
	}

	//Not entirely sure what we are using this for yet
	void Wish_ShaderProgram_Delete(wish_shader_program* program)
	{
		if (program)
		{
			if (program->GLHandle)
			{
				glDeleteProgram(program->GLHandle);
				program->GLHandle = 0;
				//Delete our error strings
				Wish_String_Free(program->ErrorString);
				//Delete the error strings of our shaders
				Wish_String_Free(program->VertexShader.ErrorString);
				Wish_String_Free(program->FragmentShader.ErrorString);
				//Free shacer uniform data too
				if (program->UniformData.NumUniforms > 0)
				{
					Wish_Memory_Free(&program->UniformData.Hashes);
					Wish_Memory_Free(&program->UniformData.Locations);
					for (u32 i = 0; i < program->UniformData.NumUniforms; i++)
					{
						Wish_String_Free(program->UniformData.Names[i]);
						Wish_String_Free(program->UniformData.Types[i]);
					}
					Wish_Memory_Free(&program->UniformData.Names);
					Wish_Memory_Free(&program->UniformData.Types);
				}
			}
		}

	}

	void Wish_ShaderProgram_Bind(wish_shader_program* program) {
		if (program && program->GLHandle)
		{
			glUseProgram(program->GLHandle);
		}
		else
		{
			glUseProgram(0);
		}
	}

	//TODO(Meanzie): Revisit usage of std::string, especially down there with the substr's
	//Like running malloc here is fine and stuff, since we are kind of destructing it when the function
	//runs out of scope, but you know, id like to handle the memory on my own :D
	wish_shader_program* Wish::Wish_ShaderProgram_Parse(wish_shader_program* program, const char* fileName) {
		//TODO(Meanzie): We are allocating this using new aka malloc, so don't do that.
		wish_string wsrc = Wish_IO_ReadFileToString(fileName);

		//Not entirely sure how smart this is but... WE ARE DOING IT
		std::string src(Wish_CString(wsrc));

		//Let's find the vertex code!
		size_t vert = src.find("#vert");
		//Let's find the fragment code
		size_t frag = src.find("#frag");

		if (vert == std::string::npos) {
			throw WishIOException("Could not find #vert tag");
		}

		if (frag == std::string::npos) {
			throw WishIOException("Could not find #frag tag");
		}

		size_t vertEnd, fragEnd;

		if (vert < frag) {
			vertEnd = frag;
			fragEnd = src.size();
		}
		else {
			vertEnd = src.size();
			fragEnd = vert;
		}

		//These are stack allocs, right =D?
		std::string fragCode = src.substr(frag + 5, (fragEnd - frag - 5));
		std::string vertCode = src.substr(vert + 5, (vertEnd - vert - 5));

		//Shader program time!
		program->VertexShader.Source = vertCode.c_str();
		program->VertexShader.ShaderType = GL_VERTEX_SHADER;
		if (Wish_Shader_Compile(&program->VertexShader))
		{
			//Shader program time!
			program->FragmentShader.Source = fragCode.c_str();
			program->FragmentShader.ShaderType = GL_FRAGMENT_SHADER;
			if (Wish_Shader_Compile(&program->FragmentShader))
			{

				//
				if (Wish_ShaderProgram_Link(program))
				{
					//Start processing shader variables
					//Find all uniforms
					size_t uniformSkipLength = strlen("uniform");
					size_t uniformPos = src.find("uniform");

					//Count before allocation
					u32 uniformCount = 0;
					while (uniformPos != std::string::npos) {
						uniformCount++;
						uniformPos = src.find("uniform", uniformPos + uniformSkipLength);
					}

					//Allocation
					program->UniformData.NumUniforms = uniformCount;
					program->UniformData.Hashes = (wish_murmur3_hash*)Wish_Memory_Alloc(sizeof(wish_murmur3_hash)*uniformCount);
					program->UniformData.Locations = (i32*)Wish_Memory_Alloc(sizeof(u32)*uniformCount);
					program->UniformData.Names = (wish_string*)Wish_Memory_Alloc(sizeof(u32)*uniformCount);
					program->UniformData.Types = (wish_string*)Wish_Memory_Alloc(sizeof(u32)*uniformCount);

					//Go back to the first occurence
					uniformPos = src.find("uniform");
					u32 cursor = 0;

					//In all fairness we could use for(i) here since we know count
					while (uniformPos != std::string::npos) {

						//Find the type of uniform
						//TODO(Meanz): Trim these searches "uniform mat4   VAR;" will break
						size_t typeEnd = src.find(" ", uniformPos + uniformSkipLength + 1);

						if (typeEnd == std::string::npos) {
							throw WishIOException("Could not find typeEnd while parsing shader");
						}

						//Another allocation of dyn size
						std::string type = src.substr(uniformPos + uniformSkipLength + 1, typeEnd - (uniformPos + uniformSkipLength + 1));

						size_t varEnd = src.find(";", typeEnd);
						size_t varEnd2 = src.find("[", typeEnd);
						if (varEnd2 != string::npos && varEnd2 < varEnd) {
							varEnd = varEnd2;
						}

						if (varEnd == std::string::npos) {
							throw WishIOException("Could not find varEnd while parsing shader");
						}

						std::string name = src.substr(typeEnd + 1, (varEnd - typeEnd - 1));

						//printf("Shader uniform %s :: %s\n", type.c_str(), name.c_str());

						//Add the uniform
						program->UniformData.Names[cursor] = name.c_str();
						program->UniformData.Types[cursor] = type.c_str();
						program->UniformData.Hashes[cursor] = Wish_Hash(name.c_str());
						program->UniformData.Locations[cursor] = glGetUniformLocation(program->GLHandle, name.c_str());

						//Find the next uniform 
						cursor++;
						uniformPos = src.find("uniform", uniformPos + uniformSkipLength);
					}
				}
				else
				{
					//TODO(Meanzie): Better error handling system than the console
					printf("Could not link program: %s \n", Wish_CString(program->ErrorString));
					Wish_String_Free(program->FragmentShader.ErrorString);
				}

				//Delete shaders
				glDeleteShader(program->VertexShader.GLHandle);
				glDeleteShader(program->FragmentShader.GLHandle);

				//We are not passing them by address so opengl won't reset them, so make sure we reset them
				program->VertexShader.GLHandle = 0;
				program->FragmentShader.GLHandle = 0;
			}
			else
			{
				//TODO(Meanzie): Better error handling system than the console
				printf("Could not compile FS: %s \n", Wish_CString(program->FragmentShader.ErrorString));
				Wish_String_Free(program->FragmentShader.ErrorString);
			}
		}
		else
		{
			//TODO(Meanzie): Better error handling system than the console
			printf("Could not compile VS: %s \n", Wish_CString(program->VertexShader.ErrorString));
			Wish_String_Free(program->VertexShader.ErrorString);
		}

		//clean up
		Wish_String_Free(program->VertexShader.Source);
		Wish_String_Free(program->FragmentShader.Source);
		Wish_String_Free(wsrc);
		return program;
	}
}
