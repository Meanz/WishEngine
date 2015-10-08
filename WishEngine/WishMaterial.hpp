#pragma once

#include "WishTexture.hpp"

namespace Wish
{

	//At the moment this works as a storage container, I don't think we like this
	//However if we make it a reference container, and one of the references dies, then what?
	//So basically if we want this to be a reference container we need to make sure we track the references
	//But then again we want to support non reference containers too
	//So what to we do senpai?
	//TODO: Revise the above thought

	//Basically connects a set of shaders and values
	struct wish_material
	{
		//Move this out of here
		mat4* World;

		//
		wish_texture* Albedo;

		//
		wish_texture* NormalMap;

		//

		//Lambert
		v3 Diffuse;
		v3 Specular;
		r32 SpecularIntensity;


		/*
		std::map<std::string, GLfloat> m_mFloat;
		std::map<std::string, GLint> m_mInt;
		std::map<std::string, GLuint> m_mUint;
		std::map<std::string, GLboolean> m_mBool;

		std::map<std::string, vec2> m_mVec2;
		std::map<std::string, vec3> m_mVec3;
		std::map<std::string, vec4> m_mVec4;

		std::map<std::string, mat3> m_mMat3;
		std::map<std::string, mat4> m_mMat4;

		std::map<std::string, wish_texture*> m_mTextures;*/
	};

	/*

	void Wish_Material_Set(wish_material* material, std::string key, const GLfloat& value);
	void Wish_Material_Set(wish_material* material, std::string key, const GLint& value);
	void Wish_Material_Set(wish_material* material, std::string key, const GLuint& value);
	void Wish_Material_Set(wish_material* material, std::string key, const GLboolean& value);

	void Wish_Material_Set(wish_material* material, std::string key, const vec2& value);
	void Wish_Material_Set(wish_material* material, std::string key, const vec3& value);
	void Wish_Material_Set(wish_material* material, std::string key, const vec4& value);

	void Wish_Material_Set(wish_material* material, std::string key, const mat3& value);
	void Wish_Material_Set(wish_material* material, std::string key, const mat4& value);

	void Wish_Material_Set(wish_material* material, std::string key, wish_texture* value);

	bool Wish_Material_Get(wish_material* material, std::string key, GLfloat* value);
	bool Wish_Material_Get(wish_material* material, std::string key, GLint* value);
	bool Wish_Material_Get(wish_material* material, std::string key, GLuint* value);
	bool Wish_Material_Get(wish_material* material, std::string key, GLboolean* value);

	bool Wish_Material_Get(wish_material* material, std::string key, vec2* value);
	bool Wish_Material_Get(wish_material* material, std::string key, vec3* value);
	bool Wish_Material_Get(wish_material* material, std::string key, vec4* value);

	bool Wish_Material_Get(wish_material* material, std::string key, mat3* value);
	bool Wish_Material_Get(wish_material* material, std::string key, mat4* value);

	wish_texture* Wish_Material_GetTexture(wish_material* material, std::string key);

	*/

}