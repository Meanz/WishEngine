#pragma once

namespace Wish
{

	struct wish_shader_uniform_data
	{
		std::vector<String> m_vUniformTypes;
		std::vector<String> m_vUniforms;
		std::vector<wish_murmur3_hash> m_vHashedUniforms;
		std::vector<GLint> m_vUniformLocations;
	};

	enum ShaderType
	{
		FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
		VERTEX_SHADER = GL_VERTEX_SHADER,
		GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
		UNSPECIFIED_SHADER = 0,
	};


	struct wish_shader
	{
		GLuint glHandle;
		ShaderType m_ShaderType;
		std::string m_Source;
		std::string m_ErrorString;
	};

	bool Wish_Shader_Compile(wish_shader* shader, ShaderType shaderType);

	struct wish_shader_program
	{
		GLuint glHandle;
		std::vector<wish_shader*> m_vAttachedShaders;
		wish_shader_uniform_data m_UniformData;
		std::string m_ErrorString;
	};

	void Wish_ShaderProgram_Attach(wish_shader_program* shaderProgram, wish_shader* pShader);
	bool Wish_ShaderProgram_Compile(wish_shader_program* shaderProgram);
	void Wish_ShaderProgram_Delete(wish_shader_program* shaderProgram);
	void Wish_ShaderProgram_Bind(wish_shader_program* shaderProgram);

	//void SetUniform(Uniform uniform);
	wish_shader_program* Wish_Shader_Load(String fragFile, String vertFile);
	wish_shader_program* Wish_ShaderProgram_Parse(wish_shader_program* pShaderProgram, String fileName);


}