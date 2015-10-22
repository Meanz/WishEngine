#pragma once

#include "stdafx.h"
#include "WishGameObject.h"

namespace Wish
{
	enum LightType
	{
		LIGHT_POINT,
		LIGHT_DIRECTIONAL,
		LIGHT_SPOTLIGHT
	};

	class wish_light : public wish_transform
	{
	public:
		wish_light(const char* name);
		~wish_light();
		
		LightType LightType;
		//Position?
		vec3 Position;
		//Color
		vec3 Diffuse;
		GLfloat DiffuseIntensity;
		//Need moar details?
		vec3 Specular;
		GLfloat SpecularIntensity;
	};

	class wish_point_light : public wish_light
	{
	public:

		wish_point_light(const char* name);
		~wish_point_light();

		GLfloat AttenuationLinear;
		GLfloat AttenuationExp;
		GLfloat AttenuationConstant;
	};
}