#pragma once

namespace Wish
{

	struct wish_shader_uniform_data
	{
		u32 NumUniforms;
		wish_murmur3_hash* Hashes;
		i32* Locations;
		wish_string* Names;
		wish_string* Types;
	};

	struct wish_shader
	{
		GLuint GLHandle;
		u32 ShaderType;
		wish_string Source;
		wish_string ErrorString;
	};

	struct wish_shader_program
	{
		GLuint GLHandle;
		wish_shader FragmentShader;
		wish_shader VertexShader;
		wish_shader_uniform_data UniformData;
		wish_string ErrorString;
	};


	b32 Wish_Shader_Compile(wish_shader* shader);
	b32 Wish_ShaderProgram_Link(wish_shader_program* program);
	void Wish_ShaderProgram_Delete(wish_shader_program* program);
	void Wish_ShaderProgram_Bind(wish_shader_program* program);
	wish_shader_program* Wish_ShaderProgram_Parse(wish_shader_program* pShaderProgram, const char* fileName);


}