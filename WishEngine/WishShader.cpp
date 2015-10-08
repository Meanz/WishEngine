#include "stdafx.h"
#include "WishCore.h"
#include "WishHash.hpp"
#include "WishShader.hpp"

//Special
#include "WishIO.hpp"

inline void DeleteShaderProgram(GLuint id) {
	if (id > 0)
		glDeleteProgram(id);
}

inline void DeleteShader(GLuint id) {
	if (id > 0)
		glDeleteShader(id);
}

namespace Wish
{
	bool Wish::Wish_Shader_Compile(wish_shader* shader, ShaderType shaderType)
	{
		ASSERT((shader->m_ShaderType != UNSPECIFIED_SHADER));

		DeleteShader(shader->glHandle);

		shader->glHandle = glCreateShader(shader->m_ShaderType);

		ASSERT((shader->glHandle > 0));

		const GLchar* src = (const GLchar*)shader->m_Source.c_str();
		glShaderSource(shader->glHandle, 1, &src, 0); //Copy the string into a GLObject

		//perform gl compile
		glCompileShader(shader->glHandle);

		//Perform error checking
		GLint isCompiled = 0;
		glGetShaderiv(shader->glHandle, GL_COMPILE_STATUS, (int*)&isCompiled);

		if (isCompiled == GL_FALSE)
		{
			//Store error string!
			GLint maxLength = 0;
			glGetShaderiv(shader->glHandle, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shader->glHandle, maxLength, &maxLength, &infoLog[0]);
			shader->m_ErrorString = (char*)&infoLog[0];
		}
		return isCompiled == GL_TRUE;
	}

	void Wish_ShaderProgram_Attach(wish_shader_program* shaderProgram, wish_shader* pShader) {
		shaderProgram->m_vAttachedShaders.push_back(pShader);
	}

	bool Wish_ShaderProgram_Compile(wish_shader_program* shaderProgram) {
		DeleteShaderProgram(shaderProgram->glHandle);

		shaderProgram->glHandle = glCreateProgram();

		ASSERT((shaderProgram->glHandle > 0));

		for (size_t i = 0; i < shaderProgram->m_vAttachedShaders.size(); i++) {
			glAttachShader(shaderProgram->glHandle, shaderProgram->m_vAttachedShaders[i]->glHandle);
		}

		glLinkProgram(shaderProgram->glHandle);

		GLint isLinked = 0;
		glGetProgramiv(shaderProgram->glHandle, GL_LINK_STATUS, (int*)&isLinked);

		if (isLinked == GL_FALSE)
		{
			//Store error string!
			GLint maxLength = 0;
			glGetProgramiv(shaderProgram->glHandle, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(shaderProgram->glHandle, maxLength, &maxLength, &infoLog[0]);
			shaderProgram->m_ErrorString = (char*)&infoLog[0];
		}

		//Delete the shader resources
		for (size_t i = 0; i < shaderProgram->m_vAttachedShaders.size(); i++) {
			glDetachShader(shaderProgram->glHandle, shaderProgram->m_vAttachedShaders[i]->glHandle);
			delete shaderProgram->m_vAttachedShaders[i];
		}
		shaderProgram->m_vAttachedShaders.clear();

		return isLinked == GL_TRUE;
	}

	void Wish_ShaderProgram_Delete(wish_shader_program* shaderProgram) {
		DeleteShaderProgram(shaderProgram->glHandle);
	}

	void Wish_ShaderProgram_Bind(wish_shader_program* shaderProgram) {
		glUseProgram(shaderProgram->glHandle);
	}

	wish_shader_program* Wish_Shader_Load(String fragFile, String vertFile)
	{
		String fragSrc, vertSrc;
		try {
			fragSrc = ReadFileToString(fragFile.c_str());
		}
		catch (WishIOException& e) {
			printf("Could not parse shader %s IOException was thrown :: %s\n", fragFile.c_str(), e.what());
			return NULL;
		}
		try {
			vertSrc = ReadFileToString(vertFile.c_str());
		}
		catch (WishIOException& e) {
			printf("Could not parse shader %s IOException was thrown :: %s\n", vertFile.c_str(), e.what());
			return NULL;
		}



		//Shader program time!
		wish_shader* pFragmentShader = new wish_shader();
		pFragmentShader->m_Source = ReadFileToString(fragFile.c_str());
		pFragmentShader->m_ShaderType = ShaderType::FRAGMENT_SHADER;

		if (!Wish_Shader_Compile(pFragmentShader, ShaderType::FRAGMENT_SHADER)) {
			printf("Unable to compile fragment shader: %s\n", pFragmentShader->m_ErrorString.c_str());
			delete pFragmentShader;
			return 0;
		}

		wish_shader* pVertexShader = new wish_shader();
		pVertexShader->m_Source = ReadFileToString(vertFile.c_str());
		pVertexShader->m_ShaderType = ShaderType::VERTEX_SHADER;

		if (!Wish_Shader_Compile(pVertexShader, ShaderType::VERTEX_SHADER)) {
			printf("Unable to compile vertex shader: %s\n", pVertexShader->m_ErrorString.c_str());
			delete pFragmentShader;
			delete pVertexShader;
			return 0;
		}

		wish_shader_program* pShaderProgram = new wish_shader_program();
		Wish_ShaderProgram_Attach(pShaderProgram, pFragmentShader);
		Wish_ShaderProgram_Attach(pShaderProgram, pVertexShader);

		if (!Wish_ShaderProgram_Compile(pShaderProgram)) {
			printf("Unable to link shaders: %s\n", pShaderProgram->m_ErrorString.c_str());
			//delete pFragmentShader;
			//delete pVertexShader;
			//^- Those are auto deleted by the shaderprogram
			delete pShaderProgram;
			return 0;
		}
		return pShaderProgram;
	}


	wish_shader_program* Wish::Wish_ShaderProgram_Parse(wish_shader_program* pShaderProgram, String fileName) {
		try {
			String src = ReadFileToString(fileName.c_str());


			//Let's find the vertex code!
			size_t vert = src.find("#vert");
			//Let's find the fragment code
			size_t frag = src.find("#frag");

			if (vert == String::npos) {
				throw WishIOException("Could not find #vert tag");
			}

			if (frag == String::npos) {
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

			String fragCode = src.substr(frag + 5, (fragEnd - frag - 5));
			String vertCode = src.substr(vert + 5, (vertEnd - vert - 5));

			//Try compiling


			//Shader program time!
			wish_shader* pFragmentShader = new wish_shader();
			pFragmentShader->m_Source = fragCode;
			pFragmentShader->m_ShaderType = ShaderType::FRAGMENT_SHADER;

			if (!Wish_Shader_Compile(pFragmentShader, ShaderType::FRAGMENT_SHADER)) {
				printf("Unable to compile fragment shader: %s\n", pFragmentShader->m_ErrorString.c_str());
				delete pFragmentShader;
				return 0;
			}

			wish_shader* pVertexShader = new wish_shader();
			pVertexShader->m_Source = vertCode;
			pVertexShader->m_ShaderType = ShaderType::VERTEX_SHADER;

			if (!Wish_Shader_Compile(pVertexShader, ShaderType::VERTEX_SHADER)) {
				printf("Unable to compile vertex shader: %s\n", pVertexShader->m_ErrorString.c_str());
				delete pFragmentShader;
				delete pVertexShader;
				return 0;
			}

			Wish_ShaderProgram_Attach(pShaderProgram, pFragmentShader);
			Wish_ShaderProgram_Attach(pShaderProgram, pVertexShader);

			if (!Wish_ShaderProgram_Compile(pShaderProgram)) {
				printf("Unable to link shaders: %s\n", pShaderProgram->m_ErrorString.c_str());
				//delete pFragmentShader;
				//delete pVertexShader;
				//^- Those are auto deleted by the shaderprogram
				delete pShaderProgram;
				return 0;
			}

			//Find all uniforms
			size_t uniformSkipLength = String("uniform").length();
			size_t uniformPos = src.find("uniform");
			while (uniformPos != String::npos) {

				//Find the type of uniform
				//TODO(Meanz): Trim these searches "uniform mat4   VAR;" will break
				size_t typeEnd = src.find(" ", uniformPos + uniformSkipLength + 1);

				if (typeEnd == String::npos) {
					throw WishIOException("Could not find typeEnd while parsing shader");
				}

				String type = src.substr(uniformPos + uniformSkipLength + 1, typeEnd - (uniformPos + uniformSkipLength + 1));

				size_t varEnd = src.find(";", typeEnd);
				size_t varEnd2 = src.find("[", typeEnd);
				if (varEnd2 != string::npos && varEnd2 < varEnd) {
					varEnd = varEnd2;
				}

				if (varEnd == String::npos) {
					throw WishIOException("Could not find varEnd while parsing shader");
				}

				String name = src.substr(typeEnd + 1, (varEnd - typeEnd - 1));

				//printf("Shader uniform %s :: %s\n", type.c_str(), name.c_str());

				//Add the uniform
				pShaderProgram->m_UniformData.m_vUniforms.push_back(name);
				pShaderProgram->m_UniformData.m_vUniformTypes.push_back(type);
				pShaderProgram->m_UniformData.m_vHashedUniforms.push_back(Wish_Hash(name.c_str()));
				//Find the next uniform 
				uniformPos = src.find("uniform", uniformPos + uniformSkipLength);
			}
		}
		catch (WishIOException& e) {
			printf("Could not parse shader %s IOException was thrown :: %s\n", fileName.c_str(), e.what());
		}

		//Find the uniforms
		if (pShaderProgram->glHandle > 0) {
			//Should be valid?
			wish_shader_uniform_data& sud = pShaderProgram->m_UniformData;

			sud.m_vUniformLocations.resize(sud.m_vUniforms.size());

			for (size_t i = 0; i < sud.m_vUniforms.size(); i++) {
				sud.m_vUniformLocations[i] = glGetUniformLocation(pShaderProgram->glHandle, (GLchar*)sud.m_vUniforms[i].c_str());
			}
		}

		return pShaderProgram;
	}

}
