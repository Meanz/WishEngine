#pragma once

namespace Wish
{
	struct wish_material
	{
		//
		wish_texture* Albedo;

		//
		wish_texture* NormalMap;

		//
		v4 Color;

		//Lambert
		v3 Diffuse;
		v3 Specular;
		r32 SpecularIntensity;
	};

	struct wish_shader_program;
	class wish_light;

	void Wish_Renderer_ApplyMaterial(wish_shader_program* program, wish_material* material);
	void Wish_Renderer_ApplyUniforms(wish_shader_program* pShaderProgram, wish_material* pMaterial, wish_light* pLight);

}