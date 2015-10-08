#pragma once

#include "stdafx.h"
#include "WishGameObject.hpp"

namespace Wish
{
	enum LightType
	{
		LIGHT_POINT,
		LIGHT_DIRECTIONAL,
		LIGHT_SPOTLIGHT
	};

	struct wish_light
	{
		wish_transform Transform;

		LightType m_LightType;
		//Position?
		vec3 Position;
		//Color
		vec3 m_Diffuse;
		GLfloat m_DiffuseIntensity;
		//Need moar details?
		vec3 m_Specular;
		GLfloat m_SpecularIntensity;
	};

	struct wish_point_light
	{
		wish_light base;
		GLfloat m_AttenuationLinear;
		GLfloat m_AttenuationExp;
		GLfloat m_AttenuationConstant;
	};
}